require 'rubygems'
require 'rake'

begin
  require 'jeweler'
  tasks = Jeweler::Tasks.new do |gem|
    gem.name = "spotlight"
    gem.summary = %Q{Spotlight for Ruby}
    gem.description = %Q{Library to use Spotlight from Ruby}
    gem.email = "youpy@buycheapviagraonlinenow.com"
    gem.homepage = "http://github.com/youpy/ruby-spotlight"
    gem.authors = ["youpy"]
    gem.files = FileList['lib/**/*.rb', '**/*.c', '**/*.h'].to_a
    gem.add_dependency "plist"
    gem.add_development_dependency "rspec", ">= 1.2.9"
    gem.add_development_dependency "rake-compiler"
    # gem is a Gem::Specification... see http://www.rubygems.org/read/chapter/20 for additional settings
    gem.extensions = FileList["ext/**/extconf.rb"]
  end
  Jeweler::GemcutterTasks.new

  require 'rake/extensiontask'
  Rake::ExtensionTask.new('md_item_native', tasks.gemspec)
  Rake::ExtensionTask.new('md_query_native', tasks.gemspec)
rescue LoadError
  puts "Jeweler (or a dependency) not available. Install it with: gem install jeweler"
end

require 'spec/rake/spectask'
Spec::Rake::SpecTask.new(:spec) do |spec|
  spec.libs << 'lib' << 'spec'
  spec.spec_files = FileList['spec/**/*_spec.rb']
end

Spec::Rake::SpecTask.new(:rcov) do |spec|
  spec.libs << 'lib' << 'spec'
  spec.pattern = 'spec/**/*_spec.rb'
  spec.rcov = true
end

task :spec => [:check_dependencies, :compile]
task :default => :spec

require 'rake/rdoctask'
Rake::RDocTask.new do |rdoc|
  version = File.exist?('VERSION') ? File.read('VERSION') : ""

  rdoc.rdoc_dir = 'rdoc'
  rdoc.title = "md_query #{version}"
  rdoc.rdoc_files.include('README*')
  rdoc.rdoc_files.include('lib/**/*.rb')
end
