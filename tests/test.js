const addon = require('../build/Release/addon');
const temp = require('temp');
const path = require('path');

temp.track();

const testfiles = path.resolve(__dirname, "testfiles");

test('simple', done => {
    temp.mkdir("simple", (err, dir) => {

        const from = path.resolve(testfiles, "simple.txt");
        const to = path.resolve(dir, "simple.txt");
        addon.win32CopyFile(
            from, to, (err) => {
            expect(err).toBeUndefined();
            done();
        });
    });
});

test('unicode', done => {
    temp.mkdir("unicode", (err, dir) => {
        const from = path.resolve(testfiles, "hello_ÆØÅ.txt");
        const to = path.resolve(dir, "hello_ÆØÅ.txt");
        addon.win32CopyFile(
            from, to, (err) => {
            expect(err).toBeUndefined();
            done();
        });
    });
});

test('filenotfound', done => {
    temp.mkdir("filenotfound", (err, dir) => {
        const from = path.resolve(testfiles, "does_not_exist.txt");
        const to = path.resolve(dir, "does_not_exist.txt");
        addon.win32CopyFile(
            from, to, (err) => {
            expect(err).toBeDefined();
            expect(err.message).toEqual("The system cannot find the file specified.");
            done();
        });
    });
});

test('invaliddest', done => {
    temp.mkdir("invaliddest", (err, dir) => {
        const from = path.resolve(testfiles, "does_not_exist.txt");
        const to = "C:\\Windows\\test.txt"
        addon.win32CopyFile(
            from, to, (err) => {
            expect(err).toBeDefined();
            expect(err.message).toEqual("The system cannot find the file specified.");
            done();
        });
    });
});
