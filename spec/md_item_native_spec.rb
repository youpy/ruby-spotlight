require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe Spotlight::MDItemNative do
  before do
    @item = Spotlight::MDItemNative.new(__FILE__)
    @filename = File.basename(__FILE__)
  end

  it "should instanciate" do
    @item.should be_an_instance_of(Spotlight::MDItemNative)
  end

  it "should get attribute by name" do
    @item.get('kMDItemKind').should eql('Ruby Source File')
    @item.get(:kMDItemKind).should eql('Ruby Source File')
    @item.get(:kMDItemFSName).should eql(@filename)
    @item.get(:kMDItemFSCreationDate).should be_an_instance_of(Time)
    @item.get(:kMDItemFSOwnerUserID).should be_an_instance_of(Fixnum)
    @item.get(:kMDItemFSIsExtensionHidden).should be_an_instance_of(FalseClass)
  end

  it "should get nil if attribute is not found" do
    @item.get('kMDXXXXXX').should be_nil
  end

  it "should get attribute names" do
    attribute_names = @item.attribute_names
    attribute_names.should be_an_instance_of(Array)
    attribute_names.should be_include('kMDItemKind')
  end
end
