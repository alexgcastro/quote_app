module.exports = function(grunt) {

    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),

        copy: {
            dev: {
                files: [
                    {expand: true, src: ['app/index.html', 'app/quote.css'],
                     dest: 'build/ui', flatten: true},
                    {expand: true, src: ['app/quote_service.js'],
                     dest: 'build', flatten: true},
                    {expand: true, src: ['text_rendering_addon/black_texture_tile.png'],
                     dest: 'build', flatten: true}
                ]
            }
        },

        concat: {
            dev: {
                files: {
                    'build/ui/quote.js':
                    ['bower_components/underscore/underscore.js',
                     'bower_components/jquery/dist/jquery.js',
                     'bower_components/backbone/backbone.js',
                     'app/quote.js']
                }
            }
        },

        clean: {
            dev: {
                src: ['build', 'node_modules', 'bower_components']
            }
        }

    });

    grunt.loadNpmTasks('grunt-contrib-copy');
    grunt.loadNpmTasks('grunt-contrib-concat');
    grunt.loadNpmTasks('grunt-contrib-clean');

    grunt.registerTask('cdirectory', 'Create generated images directory', function() {
        grunt.file.mkdir('build/generated_images')
    });

    grunt.registerTask('default', ['copy', 'concat', 'cdirectory']);
};
