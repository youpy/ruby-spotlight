#!/usr/bin/env rake
require 'rake'
require 'rake/clean'
require "bundler/gem_tasks"

ext_names = %w/md_query_native md_item_native/
ext_names.each do |ext_name|
  CLEAN.include Dir.glob("ext/#{ext_name}/*{.o,.log}")
  CLEAN.include "ext/#{ext_name}/Makefile"
  CLOBBER.include "ext/#{ext_name}/#{ext_name}.bundle"
  CLOBBER.include "lib/#{ext_name}.bundle"

  file "lib/#{ext_name}.bundle" =>
    Dir.glob("ext/#{ext_name}/*{.rb,.m}") do
    Dir.chdir("ext/#{ext_name}") do
      ruby "extconf.rb"
      sh "make"
    end
    cp "ext/#{ext_name}/#{ext_name}.bundle", "lib/"
  end

  task :spec => "lib/#{ext_name}.bundle"
end

require 'rspec/core'
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

task :default => :spec
