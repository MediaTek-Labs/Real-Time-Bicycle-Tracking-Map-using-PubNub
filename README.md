# Real-Time-Bicycle-Tracking-Map-Using-PubNub
Bicycle tracker utilizes the [LinkIt ONE development board’s](http://labs.mediatek.com/oneguide) GNSS features to acquire the 
position of a bike in real-time. The data is then uploaded to the [EON real-time dashboard](http://www.pubnub.com/developers/eon/) 
over a GPRS connection using the [PubNub LinkIt ONE library](https://github.com/pubnub/LinkItONE) support. The EON real-time dashboard provides a visualization of the data on a map.

### Before You Start 

The project requires user accounts for [PubNub](https://www.pubnub.com/) and [Mapbox](https://www.mapbox.com/projects/).  Real-time bike tracking map requires libraries from [eon-map](https://github.com/pubnub/eon-map). 
Full details on downloading and installing the Arduino IDE and LinkIt ONE SDK then configuring the IDE and upgrading the board firmware are provided in the [LinkIt ONE quick start guide](http://labs.mediatek.com/site/global/developer_tools/mediatek_linkit/get-started/index.gsp). Complete this before you continue if you haven’t already set up your development environment. 

