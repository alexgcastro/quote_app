Quote application
=================

Quoting is fun.

This application creates quote images from text.

How to use it
-------------

You need to install node and npm packages in your system, for Fedora you can:

yum install nodejs npm

After that we have to install the dependencies for development and
execution of the package, check package.json for more information:

npm install

To make sure you are using the development packages installed by npm
using you need to add a PATH:

export PATH=./node_modules/.bin:$PATH

Build the project:

grunt

Run it using node:

node build/quote_service.js
