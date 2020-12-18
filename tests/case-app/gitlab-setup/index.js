"use strict";

const puppeteer = require("puppeteer");
const logger = require("pino")();
const rl = require('readline');
const child = require("child_process");
const fse = require("fs-extra");
const path = require("path");

const GITLAB_DOMAIN = "gitlab.custom.local";
const GITLAB_URL = "http://" + GITLAB_DOMAIN;
const GITLAB_USER = "root";
const GITLAB_PASSWORD = "6sGAteviPyQ.kDcnZDv7";
const GITLAB_PROJECT_NAME = "case-app";
const GITLAB_PROJECT_SLUG = GITLAB_PROJECT_NAME;
const REPO_PARENT = `${process.env.HOME}/.virtualization-tests`;
const REPO_TARGET = REPO_PARENT + "/case-repo";
const SSH_KEY = "gitlab_ed25519";
const SSH_EMAIL = "example@example.com";
const SSH_CONFIG = `
Host ${GITLAB_DOMAIN}
  StrictHostKeyChecking no
  AddKeysToAgent yes
  UseKeychain yes
  IdentityFile ~/.ssh/${SSH_KEY}

`;

/**
 * Register the GitLab root user.
 * 
 * @param {puppeteer.Browser} browser The browser
 */
async function registerRoot(browser) {
    const FORM_SELECTOR = "#new_user";
    const PASSWORD_SELECTOR = "#user_password";
    const PASSWORD_CONFIRMATION_SELECTOR = "#user_password_confirmation";
    const FLASH_PATH = "body > div > div.container.navless-container > div > div.flash-container.flash-container-page.sticky > div > span";
    const FLASH_TEXT = "Your password has been changed successfully.";
    const page = await browser.newPage();
    await page.goto(GITLAB_URL)
    // Insert password
    await page.$eval(PASSWORD_SELECTOR, (el, val) => el.value = val, GITLAB_PASSWORD);
    await page.$eval(PASSWORD_CONFIRMATION_SELECTOR, (el, val) => el.value = val, GITLAB_PASSWORD);
    // Submit and wait for navigation
    await page.$eval(FORM_SELECTOR, (form) => form.submit());
    await page.waitForNavigation();
    const success = await page.$eval(FLASH_PATH, (el, text) => el.innerText === text, FLASH_TEXT);
    if (!success) {
        await page.screenshot({ path: "error.png" });
        throw new Error("Registering root failed, check error.png for details");
    }
    await page.close();
}

/**
 * Login to GitLab.
 * 
 * @param {puppeteer.Browser} browser The browser
 * 
 * @return {Promise<puppeteer.Page>} The page
 */
async function login(browser) {
    const USERNAME_SELECTOR = "#user_login";
    const PASSWORD_SELECTOR = "#user_password";
    const FORM_SELECTOR = "#new_user";
    const ADMIN_AREA_PATH = "body > header > div > div > div.title-container > ul > li:nth-child(5) > a";
    const ADMIN_AREA_LABEL = "Admin Area";
    const page = await browser.newPage();
    await page.goto(GITLAB_URL + "/users/sign_in");
    await page.$eval(USERNAME_SELECTOR, (e, val) => e.value = val, GITLAB_USER);
    await page.$eval(PASSWORD_SELECTOR, (e, val) => e.value = val, GITLAB_PASSWORD);
    await page.$eval(FORM_SELECTOR, (form) => form.submit());
    await page.waitForNavigation();
    const success = await page.$eval(ADMIN_AREA_PATH, (el, label) => el.getAttribute("aria-label") === label, ADMIN_AREA_LABEL);
    if (!success) {
        await page.screenshot({ path: "error.png" });
        throw new Error("Login failed, check error.png for details");
    }
    return page;
}

/**
 * Wait for user input.
 *
 * @param {string} prompt Optional prompt for the user
 * @return {Promise<void>}
 */
function waitForInput(prompt = "Press enter to continue") {
    const i = rl.createInterface({
        input: process.stdin,
        output: process.stdout,
    });
    return new Promise((resolve) => {
        i.question(prompt, () => {
            i.close();
            resolve();
        });
    });
}

/**
 * Register a SSH key for the user.
 *
 * @param {puppeteer.Browser} browser The browser
 */
async function registerSshKey(browser) {
    const KEYS_PATH = "/profile/keys";
    const KEY_LOCATION = `${process.env.HOME}/.ssh/${SSH_KEY}`;
    const KEY_SELECTOR = "#key_key";
    const KEY_TITLE_SELECTOR = "#key_title";
    const KEY_FORM_SELECTOR = "#new_key";
    const SSH_CONFIG_FILE = `${process.env.HOME}/.ssh/config`;
    // No-password ssh key
    const cmd = `ssh-keygen -f ${KEY_LOCATION} -t ed25519 -C "${SSH_EMAIL}" -q -N ""`;

    if (!fse.existsSync(KEY_LOCATION)) {
        logger.info(`No ssh key found from ${KEY_LOCATION}, creating one...`);
        const [stdout, stderr] = await new Promise((resolve, reject) => {
            child.exec(cmd, (err, stdout, stderr) => {
                if (err) {
                    return reject(err);
                }
                return resolve([stdout, stderr]);
            });
        });
        logger.info(`Key created, stdout: "${stdout}", stderr: "${stderr}"`);
        // Config for OS X
        if (!fse.existsSync(SSH_CONFIG_FILE)) {
            await fse.writeFile(SSH_CONFIG_FILE, SSH_CONFIG, { encoding: "utf-8" });
        } else {
            await fse.appendFile(SSH_CONFIG_FILE, SSH_CONFIG);
        }

    }
    const publicKeyContents = await fse.readFile(`${KEY_LOCATION}.pub`, "utf-8");
    
    const page = await browser.newPage();
    await page.goto(GITLAB_URL + KEYS_PATH);
    await page.$eval(KEY_SELECTOR, (e, val) => e.value = val, publicKeyContents);
    await page.$eval(KEY_TITLE_SELECTOR, (e, val) => e.value = val, SSH_KEY);
    await page.$eval(KEY_FORM_SELECTOR, (form) => form.submit());
    await page.waitForNavigation();
    const success = await page.evaluate((selector) => {
        const el = document.querySelector(selector);
        // If element is found, we didn't navigate to the new key page
        return !el;
    }, KEY_SELECTOR);
    if (!success) {
        await page.screenshot({ path: "error.png" });
        throw new Error("Generating SSH key failed, check error.png for details");
    }
}

/**
 * Copy the repository to temporary destination in order to be able to use it with GitLab.
 */
async function copyRepository() {
    const REPO_SOURCE = path.join(__dirname, "..", "case-repo");
    if (!fse.existsSync(REPO_PARENT)) {
        logger.info(`Creating directory ${REPO_PARENT}...`);
        await fse.mkdirp(REPO_PARENT);
    }
    if (!fse.existsSync(REPO_TARGET)) {
        logger.info(`Copying the repository from ${REPO_SOURCE} to ${REPO_TARGET}`);
        await fse.copy(REPO_SOURCE, REPO_TARGET);
    } else if (fse.existsSync(`${REPO_TARGET}/.git`)) {
        logger.info("Directory exists and contains .git -directory, removing it...");
        await fse.remove(`${REPO_TARGET}/.git`);
    }
}

/**
 * Register the case app repository into GitLab server.
 *
 * @param {puppeteer.Browser} browser The browser
 */
async function registerRepository(browser) {
    const NEW_PROJECT_URL = GITLAB_URL + "/projects/new";
    const PROJECT_NAME_SELECTOR = "#project_name";
    const PROJECT_SLUG_SELECTOR = "#project_path"
    const PROJECT_FORM_SELECTOR = "#new_project";
    const PROJECT_URL = `${GITLAB_URL}/${GITLAB_USER}/${GITLAB_PROJECT_SLUG}`;
    const page = await browser.newPage();
    await page.goto(NEW_PROJECT_URL);
    await page.$eval(PROJECT_NAME_SELECTOR, (e, v) => e.value = v, GITLAB_PROJECT_NAME);
    await page.$eval(PROJECT_SLUG_SELECTOR, (e, v) => e.value = v, GITLAB_PROJECT_SLUG);
    await page.$eval(PROJECT_FORM_SELECTOR, (e) => e.submit());
    await page.waitForNavigation();
    if (page.url() !== PROJECT_URL) {
        logger.warn(`Expected the URL to be ${PROJECT_URL} but it was ${page.url()} instead, failing...`);
        await page.screenshot({ path: "error.png" });
        throw new Error("Creating a new project failed, check error.png for details");
    }
    logger.info("Registering the repository...");
    const output = child.execSync(
        `cd ${REPO_TARGET} && git init `
        + `&& git remote add origin git@${GITLAB_DOMAIN}:${GITLAB_USER}/${GITLAB_PROJECT_SLUG}.git`
        + `&& git add . && git commit -m "Initial commit" && git push -u origin master`
    );
    logger.info(output.toString());
}

/**
 * Create a new Git repository.
 * 
 * @param {puppeteer.Browser} browser The browser
 */
async function createRepository(browser) {
    await registerSshKey(browser);
    logger.info("SSH key installed");
    await copyRepository();
    logger.info("Repository in place");
    await registerRepository(browser);
    logger.info("Repository registered");
}

/**
 * Register runner into GitLab.
 * 
 * @param {puppeteer.Browser} browser Browser
 */
async function registerRunner(browser) {
    const RUNNERS_PATH = "/admin/runners"
    const REGISTRATION_TOKEN_SELECTOR = "#registration_token";
    const REGISTRATION_URL = GITLAB_URL + "/";
    const page = await login(browser);
    logger.info("Login success");

    await page.goto(GITLAB_URL + RUNNERS_PATH);
    const registrationToken = await page.$eval(REGISTRATION_TOKEN_SELECTOR, (el) => el.textContent);
    logger.info(`Got registration token '${registrationToken}' and URL ${REGISTRATION_URL}, please register the runner...`);

    await waitForInput("Press enter once you have registered the runner: \n");
    await page.close();
}

async function main() {
    const browser = await puppeteer.launch();
    
    try {
        // Register root user
        await registerRoot(browser);
        logger.info(`GitLab account registered with username '${GITLAB_USER}' and password '${GITLAB_PASSWORD}'`);

        // Register runner - requires manual work
        await registerRunner(browser);
        logger.info("Runner registered");

        // Crete the case app repository
        await createRepository(browser);
    } catch (e) {
        logger.error(e, "Script failed");
    } finally {
        await browser.close();
    }

}

main()
    .catch(e => logger.error(e, "Script failed"));


