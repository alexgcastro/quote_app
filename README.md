Quote application
=================

Quoting is fun.

This application creates quote images from text.

How to use it
-------------

You need to install node, npm and MongoDB packages in your system, for
Fedora you can install nodejs and npm using:

`yum install nodejs npm`

For MongoDB you can use the installation instructions in the [MongoDB
webpage]("http://docs.mongodb.org/manual/tutorial/install-mongodb-on-red-hat-centos-or-fedora-linux/").

After that we have to install the dependencies for development and
execution of the package, check package.json for more information:

`npm install`

To make sure you are using the development packages installed by npm
using you need to add a PATH:

`export PATH=./node_modules/.bin:$PATH`

Build the project:

`grunt`

Run it using node:

`node build/quote_service.js`
