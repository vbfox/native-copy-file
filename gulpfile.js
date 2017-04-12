const gulp = require("gulp");
const ts = require("gulp-typescript");
const rimraf = require("rimraf");
const tslint = require("gulp-tslint");

const tsProject = ts.createProject('./tsconfig.json');

gulp.task("clean", done => {
    rimraf("js", done);
});

gulp.task("tslint", () => {
    return tsProject.src()
        .pipe(tslint({
            formatter: "verbose"
        }))
        .pipe(tslint.report())
});

gulp.task("build", () => {
    const tsResult = tsProject.src()
        .pipe(tsProject());
    return tsResult.pipe(gulp.dest('js'));
});

gulp.task("default", ["tslint", "build"], () => {});
