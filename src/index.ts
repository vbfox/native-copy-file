import Options, { Times, defaultOptions } from "./options"
import {copyFile as copyFileManual, utimes}  from "./manual";

export async function copyFile(src: string, dest: string, options?: Options) {
    const resolvedOptions: Options = Object.assign({}, defaultOptions, options);
    await copyFileManual(src, dest);
    if (resolvedOptions.times !== undefined && typeof resolvedOptions.times !== "string") {
        const times: Times = resolvedOptions.times;
        await utimes(dest, times.atime, times.mtime);
    }
}