import Options, { Times, defaultOptions } from "./options"
import {copyFile as copyFileManual, utimes}  from "./manual";

export function copyFile(src: string, dest: string, options?: Options) {
    const resolvedOptions: Options = Object.assign({}, defaultOptions, options);
    return copyFileManual(src, dest).then(() => {
        if (resolvedOptions.times !== undefined && typeof resolvedOptions.times !== "string") {
            const times: Times = resolvedOptions.times;
            return utimes(dest, times.atime, times.mtime);
        } else {
            return Promise.resolve();
        }
    });
}