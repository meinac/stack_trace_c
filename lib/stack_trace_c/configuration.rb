module StackTraceC
  class Configuration
    attr_accessor :modules
    attr_writer :trace_c, :trace_ruby

    def trace_c?
      @trace_c
    end

    def trace_ruby?
      @trace_ruby
    end
  end
end
