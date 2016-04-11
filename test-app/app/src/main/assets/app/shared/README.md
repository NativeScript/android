## TNS Shared Tests

Currently targeting the Telerik NativeScript runtimes for:
 - iOS
 - android
 
The tests should cover sommon functionality such as the "require" implementations,
basic inheritance scenarios, ECMA version etc.

[The unit tests are written in Jasmine.](http://jasmine.github.io/2.0/introduction.html)

To run the tests you should add this repo as submodule in the app/shared folder.
There is a satelite repo that contains module which should be added as submodule in tns_modules/shared respectively.

## Fixtures
`TNSLog(text)` method is expected to be defined in the global object collecting messages.
`TNSGetOutput()` method returns all messages collected in TNSLog.
`TNSClearOutput()` clears the collected messages in TNSLog.
`__approot` global variable should point the path to the root folder of the app, the one where 'app' and 'tns_modules' are located. The path should start with leading slash.

