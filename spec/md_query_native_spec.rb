require File.expand_path(File.dirname(__FILE__) + '/spec_helper')

describe MDQueryNative do
  before do
    @query = MDQueryNative.new('kMDItemDisplayName = "*_spec.rb"cdw')
  end

  it "should instanciate" do
    @query.should be_an_instance_of(MDQueryNative)
  end
end
