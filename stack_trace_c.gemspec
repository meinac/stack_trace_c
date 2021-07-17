# frozen_string_literal: true

require_relative "lib/stack_trace_c/version"

Gem::Specification.new do |spec|
  spec.name          = "stack_trace_c"
  spec.version       = StackTraceC::VERSION
  spec.authors       = ["Mehmet Emin INAC"]
  spec.email         = ["mehmetemininac@gmail.com"]

  spec.summary       = "ASD"
  spec.description   = "ASD"
  spec.homepage      = "http://www.example.com"
  spec.license       = "MIT"
  spec.required_ruby_version = Gem::Requirement.new(">= 2.7.0")

  spec.metadata["allowed_push_host"] = "asda"

  spec.metadata["homepage_uri"] = spec.homepage

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{\A(?:test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.extensions    = %w[ext/stack_trace_c/extconf.rb]
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "bundler", "~> 2.0"
  spec.add_development_dependency "pry"

  # Uncomment to register a new dependency of your gem
  # spec.add_dependency "example-gem", "~> 1.0"

  # For more information and examples about making a new gem, checkout our
  # guide at: https://bundler.io/guides/creating_gem.html
end
