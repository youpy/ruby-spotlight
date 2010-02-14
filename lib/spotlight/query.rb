module Spotlight
  class Query
    attr_reader :query_string
    attr_accessor :scopes

    def initialize(query_string)
      @query_string = query_string
      @scopes = []
    end

    def execute
      @md_query = MDQueryNative.new(query_string)
      @md_query.set_search_scopes(@scopes)
      @md_query.execute
    end
  end
end
