const gulp = require('gulp')
const exec = require('child_process').exec

gulp.task('build:game', function (done) {
  exec('make', function (err, stdout, stderr) {
    if (err) {
      console.log(stdout)
      console.log(stderr)
    }
    done()
  })
})

gulp.task('update:resources', function (done) {
  exec('make resources', function (err, stdout, stderr) {
    if (err) {
      console.log(stdout)
      console.log(stderr)
    }
    done()
  })
})

gulp.task('watch:resources', function () {
  const resourceWatch = gulp.watch('assets/**/*', ['update:resources'])
  resourceWatch.on('change', function (event) { console.log(`${event.path} was ${event.type}`) })
})

gulp.task('watch:game', function () {
  const gameSourceWatch = gulp.watch('src/**/*.cpp', ['build:game'])
  const gameHeaderWatch = gulp.watch('src/**/*.h', ['build:game'])

  gameSourceWatch.on('change', function (event) { console.log(`${event.path} was ${event.type}`) })
  gameHeaderWatch.on('change', function (event) { console.log(`${event.path} was ${event.type}`) })
})

gulp.task('watch', ['watch:game', 'watch:resources'], function () {})

gulp.task('clean:game', function (done) {
  exec('make clean', function (err, stdout, stderr) {
    if (err) {
      console.log(stdout)
      console.log(stderr)
    }
    done()
  })
})

gulp.task('clean', ['clean:game'], function () {})

gulp.task('assets', ['update:resources'], function () {})

gulp.task('default', ['build:game'], function () {})

gulp.task('run', ['build:game', 'update:resources'], function (done) {
  exec('cd ./dist && ./game && cd ..', function (err, stdout, stderr) {
    if (err) {
      console.log(stdout)
      console.log(stderr)
      exec('cd ..')
    }
    console.log(stdout)
    console.log(stderr)
    done()
  })
})
