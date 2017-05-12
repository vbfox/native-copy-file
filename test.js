const addon = require('./build/Release/addon');

test('world', done => {
    const result = addon.win32CopyFile(
        "G:\\Code\\hello_ÆØÅ.txt", "G:\\Code\\hello_ÆØÅ_copy.txt", function(ok, err) {
            console.log(ok);
            console.log(err);
            done();
        });
    //expect(result).toBeTruthy();
});
