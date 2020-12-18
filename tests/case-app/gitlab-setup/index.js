"use strict";

const puppeteer = require("puppeteer");
const logger = require("pino")();
const rl = require('readline');
const fs = require("fs");
const child = require("child_process");

const GITLAB_URL = "http://gitlab.custom.local";
const GITLAB_USER = "root";
const GITLAB_PASSWORD = "6sGAteviPyQ.kDcnZDv7";
const SSH_KEY = "gitlab_ed25519";
const SSH_EMAIL = "example@example.com";

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
    // No-password ssh key
    const cmd = `ssh-keygen -f ${KEY_LOCATION} -t ed25519 -C "${SSH_EMAIL}" -q -N ""`;

    if (!fs.existsSync(KEY_LOCATION)) {
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
    }
    const publicKeyContents = fs.readFileSync(`${KEY_LOCATION}.pub`, "utf-8");
    
    const page = await browser.newPage();
    await page.goto(GITLAB_URL + KEYS_PATH);
    await page.$eval(KEY_SELECTOR, (e, val) => e.value = val, publicKeyContents);
    await page.$eval(KEY_TITLE_SELECTOR, (e, val) => e.value = val, SSH_KEY);
    await page.$eval(KEY_FORM_SELECTOR, (form) => form.submit());
    await page.waitForNavigation();
}

/**
 * Create a new Git repository.
 * 
 * @param {puppeteer.Browser} browser The browser
 */
async function createRepository(browser) {
    await registerSshKey(browser);
    logger.info("SSH key installed");
    
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
        // await registerRoot(browser);
        logger.info(`GitLab account registered with username '${GITLAB_USER}' and password '${GITLAB_PASSWORD}'`);

        // await registerRunner(browser);
        await login(browser);
        logger.info("Runner registered");
        await createRepository(browser);
    } catch (e) {
        logger.error(e, "Script failed");
    } finally {
        await browser.close();
    }

}

main()
    .catch(e => logger.error(e, "Script failed"));


