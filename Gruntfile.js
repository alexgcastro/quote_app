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

        bower: {
            install: {
                options: {
                    targetDir: './build/ui',
                    layout: function(type, component) {
                        return "";
                    },
                    install: true,
                    verbose: false,
                    cleanTargetDir: false,
                    cleanBowerDir: false,
                    bowerOptions: {}
                }
            }
        },

        clean: {
            dev: {
                src: ['build', 'node_modules', 'bower_components']
            }
        },

        react: {
            single_file_output: {
                files: {
                    'build/ui/quote.js': 'app/quote.jsx'
                }
            }
        },

        jshint: {
            all: ['Gruntfile.js', 'app/*.js', 'build/ui/quote.js']
        }

    });

    grunt.loadNpmTasks('grunt-contrib-copy');
    grunt.loadNpmTasks('grunt-contrib-clean');
    grunt.loadNpmTasks('grunt-bower-task');
    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-react');

    grunt.registerTask('cdirectory', 'Create generated images directory', function() {
        grunt.file.mkdir('build/generated_images')
    });

    grunt.registerTask('default', ['react', 'jshint', 'copy', 'bower', 'cdirectory']);
};
