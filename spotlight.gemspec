# -*- encoding: utf-8 -*-
#require File.expand_path(__FILE__)

Gem::Specification.new do |gem|
  gem.authors       = ["youpy"]
  gem.email         = ["youpy@buycheapviagraonlinenow.com"]
  gem.description   = %q{Library to use Spotlight from Ruby}
  gem.summary       = %q{Library to use Spotlight from Ruby}
  gem.homepage      = ""

  gem.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  gem.files         = `git ls-files`.split("\n")
  gem.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  gem.name          = "spotlight"
  gem.extensions    = ["ext/md_item_native/extconf.rb", "ext/md_query_native/extconf.rb"]
  gem.require_paths = ["lib"]
  gem.version       = '0.0.6'

  gem.add_dependency('plist')

  gem.add_development_dependency('rake')
  gem.add_development_dependency('rspec', ['~> 2.8.0'])
  gem.add_development_dependency('spotlight')
end
