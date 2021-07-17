# frozen_string_literal: true

require_relative "stack_trace_c/version"
require_relative "stack_trace_c/configuration"

module StackTraceC
  def self.configure(&block)
    block.call(configuration)
  end

  def self.trace(&block)
    Trace.start
    trace_point.enable { yield }
  end

  def self.configuration
    @configuration ||= Configuration.new
  end
end

require "stack_trace_c/stack_trace_c"
