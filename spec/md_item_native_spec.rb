require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Spotlight::MDItemNative do
  before do
    @query = Spotlight::MDItemNative.new(__FILE__)
  end

  it "should instanciate" do
    @query.should be_an_instance_of(Spotlight::MDItemNative)
  end

  it "should get attribute by name" do
    @query.get('kMDItemKind').should eql('Ruby Source File')
    @query.get(:kMDItemKind).should eql('Ruby Source File')
  end

  it "should get nil if attribute is not found" do
    @query.get('kMDXXXXXX').should be_nil
  end
end
