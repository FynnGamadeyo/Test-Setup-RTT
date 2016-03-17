require 'orocos'
require 'readline'

include Orocos
Orocos.initialize


Orocos.run 'client::TaskB' => 'client__TaskB' do  

  read_msg = Orocos.name_service.get 'client__TaskB'  
  send_msg = Orocos.name_service.get 'client__TaskA'

  send_msg.messages.connect_to read_msg.messages

  read_msg.configure
  read_msg.start
  
  Readline::readline("Press ENTER to exit\n")
end
