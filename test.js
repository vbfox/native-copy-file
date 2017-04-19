const addon = require('./build/Release/addon');

test('world', () => {
    const result = addon.win32CopyFile("G:\\Code\\hello_ÆØÅ.txt", "G:\\Code\\hello_ÆØÅ_copy.txt");
    expect(result).toBeTruthy();
});
