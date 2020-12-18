# case-app

_This directory contains the tests related to the case app._

## Description

The case app is GitLab server with one pipeline runner.

## Tests

The tests are described here.

### Pipeline test

In the pipeline test, we create a sample C++ application repository and run certain automated tasks within the pipeline against that app. The tasks that are run within the pipeline include:

1. Code compilation with coverage flag
2. Code unit testing with Valgrind
3. Code coverage reporting

The feature we measure in the test is **time** it takes to run the pipeline.

The tests requires some preparations to be done:

1. Setup the GitLab server (root account password)
2. Setup the GitLab runner
3. Setup the application code repository
