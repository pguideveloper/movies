/**
 * User: Vladimir Bulyga <zero@cetku.net>
 * Project: clone-object
 * Date: 06.05.13 22:06
 */

var fs = require('fs'),
    assert = require('assert'),
    jsExtend = require('deep-extend'),
    naExtend = require('./').extend;

function test() {
    var args = [].slice.call(arguments),
        message = args.shift(),
        func = args.shift(),
        start = new Date().getTime(),
        callback = typeof args[args.length-1] === 'function' ? args.pop() : function() {};
    args.push(function() {
        console.log(">>> %s : %d sec", message, Number(((new Date().getTime())-start)/1000).toFixed(4));
        return callback.apply(this, arguments);
    });
    return func.apply(global, args);
}

function getJSON(filename, callback) {
    var json = [],
        stream = fs.createReadStream(filename),
        toJSON = new Function('str', 'return str;');
    stream.on('data', function(data) {
        data = data.toString('utf-8').split(/\n/);
        data.forEach(function(data) {
            json.push(toJSON(data));
        });
    });
    stream.on('end', function() {
        callback(json);
    });
}

function testExtend(data, useNative, callback) {
    var extend = useNative ? naExtend : jsExtend;
    callback(extend({}, data));
}

var data;
test('Loading JSON file', getJSON, './ny-nodes.json', function(data) {
    console.log('Data.length: %d', data.length);
    for (var i = 0; i < 10; i++ ) {
        test('testing native extend', testExtend, data, true, function(jsData) {
            test('testing javascript extend', testExtend, data, false, function(naData) {
//                console.log(naData);
                assert.deepEqual(data, jsData, 'data === jsData');
                assert.deepEqual(data, naData, 'data === naData');
                assert.deepEqual(jsData, jsData, 'jsData === naData');
            });
        });
    }
});

var testObject = {hello:{deep:{my: 'world'}}};
assert.deepEqual(testObject, naExtend(testObject), 'Clone deep object');

//assert.deepEqual(naExtend({}, {}), jsExtend({}, {}), '{} === {}');
//assert.deepEqual(naExtend([], []), jsExtend([], []), '[] === []');
//assert.deepEqual(naExtend({}, 'hello'), jsExtend({}, 'hello'), '"hello" === "hello"');