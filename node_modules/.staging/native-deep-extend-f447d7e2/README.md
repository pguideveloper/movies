#Native implementation of deep-extend module

#Install
```Bash
    npm install native-deep-extend
```

#Using
```JavaScript
    var Extend = require('native-deep-extend'),
           obj = {deep: {hello: ["me", "world"]}},
          func = function Test() {console.log("test function")},
        extObj = Extend.extend({my: ["variable"]}, obj),
      cloneObj = Extend.clone(obj),
     cloneFunc = Extend.clone(func);
    
    console.log(obj !== extObj);
    console.log(obj.my !== extObj.my && obj.my === undefined );
    console.log(obj !== cloneObj);
    console.log(obj.deep !== extObj.deep && obj.deep === cloneObj.deep);
    console.log(func !== cloneFunc);
```