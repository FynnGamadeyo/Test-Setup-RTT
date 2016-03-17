require 'orocos'
include Orocos

Orocos.initialize

Orocos.run 'client::TaskA' => 'client__TaskA' do

  messages = Orocos.name_service.get 'client__TaskA'
  messages.configure
  
  messages.start
  
  #reader = messages.messages.reader
  
  puts 'Starting to send messages ...'
  Readline::readline("Press ENTER to exit\n")
end
