const gulp = require("gulp");
const ts = require("gulp-typescript");
const rmdir = require("rmdir");

const tsProject = ts.createProject('./tsconfig.json');

gulp.task("clean", done => {
    rmdir("js", done);
});

gulp.task("build", () => {
    const tsResult = tsProject.src()
        .pipe(tsProject());
    return tsResult.pipe(gulp.dest('js'));
});

gulp.task("default", ["build"], () => {});

gulp.task("ci", ["clean", "build"], () => {});
