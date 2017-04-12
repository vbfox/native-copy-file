import * as fs from 'fs';

export function utimes(path: string, atime: number, mtime: number) {
    return new Promise((resolve, reject) => {
        fs.utimes(path, atime, mtime, function(err) {
            if (err) {
                reject(err);
            } else {
                resolve();
            }
        });
    });
}

export function copyFile(src: string, dest: string, mode?: number) {
    return new Promise((resolve, reject) => {
        const readStream = fs.createReadStream(src);
        const writeStream = fs.createWriteStream(dest, { mode });

        readStream.on('error', reject);
        writeStream.on('error', reject);

        writeStream.on('open', function() {
            readStream.pipe(writeStream);
        });

        writeStream.once('finish', function() {
            resolve();
        });
    });
}