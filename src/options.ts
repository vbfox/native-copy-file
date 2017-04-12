export interface Times {
    atime: number;
    mtime: number;
}

export interface Options {
    times?: "copy" | "ignore" | Times
    mode?: "copy" | "ignore" | number
}

export const defaultOptions: Options = {
    times: "copy"
}

export default Options;