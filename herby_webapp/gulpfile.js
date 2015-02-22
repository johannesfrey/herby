var gulp = require('gulp');
var compass = require('gulp-compass');
var browserify = require('browserify');
var source = require('vinyl-source-stream');

gulp.task('default', ['css', 'js']);
gulp.task('css', bundleCSS);
gulp.task('js', bundleJS);

function bundleCSS() {
  return gulp.src('./client/src/sass/*.scss')
    .pipe(compass({
      config_file: 'compass.rb',
      import_path: 'bower_components/foundation/scss',
      css: 'client/build/assets',
      sass: 'client/src/sass'
    }));
}

function bundleJS() {
  return browserify('./client/src/javascript/app.js')
    .bundle()
    .pipe(source('bundle.js'))
    .pipe(gulp.dest('./client/build/assets/'));
}
