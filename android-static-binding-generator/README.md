# android-static-binding-generator
This is a tool for javascript static analysis. 
* Input: valid javascript code.
* Output: should be generated file with necessary information that is [NativeScript](https://www.nativescript.org/) specific.

## How to install

open project folder and run: 
```
npm install
```

## Usage with Sublime Text 3

* Open `project/parse.js` in Sublime Text 3
* in Editor, select `Tools/BuildSystem/NodeJs`. You need to have [nodejs](https://nodejs.org/en/) installed. I'm using latest LTS(long term support) currently 4.2.5.
* `Alt+R` to run nodejs in editor

## How to test
* `npm install`
* `npm test`

## More information:
* babel: javasript transpiler
* [babylon](https://github.com/babel/babel/tree/master/packages/babylon): babel javascript parser
* [babel handbook](https://github.com/thejameskyle/babel-handbook/blob/master/translations/en/README.md)
* learning from [plugin handbook](https://github.com/thejameskyle/babel-handbook/blob/master/translations/en/plugin-handbook.md)


