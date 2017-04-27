import * as ffi from "ffi";

// tslint:disable-next-line
const wchar_t: any = require("ref-wchar");

const kernel32 = ffi.Library("kernel32", {
    CopyFileW: ["uint32", [wchar_t.string, wchar_t.string, "uint32"]]
});

const longFileNamePrefix = "\\\\?\\";

export function copyFile(src: string, dest: string) {
    return new Promise((resolve, reject) => {
        kernel32.CopyFileW.async(
            longFileNamePrefix + src,
            longFileNamePrefix + dest,
            0,
            (err: NodeJS.ErrnoException|undefined, res: number) => {
                if (err) {
                    reject(err);
                }

                if (res === 0) {
                    reject(new Error("CopyFileW failed"));
                }

                resolve();
            });
    });
}
