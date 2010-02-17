require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Spotlight::Query do
  before do
    @query = Spotlight::Query.new('kMDItemDisplayName = "spotlight_query_spec.rb"')
  end

  it "should create query from saved search" do
    query = Spotlight::Query.from_saved_search(fixture('test.savedSearch'))
    query.query_string.should eql('((true) && (true)) && ((* = "test*"cdw || kMDItemTextContent = "test*"cdw))')
    query.scopes.should eql(['kMDQueryScopeComputer'])
  end

  it "should have query string" do
    @query.query_string.should eql('kMDItemDisplayName = "spotlight_query_spec.rb"')
  end

  it "should execute query" do
    @query.scopes << File.expand_path(File.dirname(__FILE__))
    result = @query.execute
    result.size.should eql(1)
    result.first.should be_an_instance_of(Spotlight::MDItemNative)
    result.first.get(:kMDItemPath).should eql(File.expand_path(File.dirname(__FILE__) + '/spotlight_query_spec.rb'))

    @query.scopes = ['/tmp/xxx/yyy']
    result = @query.execute
    result.should be_empty
  end

  it "should execute query with empty scope" do
    result = @query.execute
    result.should_not be_empty 
  end
end
