use strict;
use warnings;
use Device::SerialPort qw( :PARAM :STAT 0.07 );
use Linux::Joystick;
use Time::HiRes qw ( usleep );

#For some reason, write_settings does not seem to be working,
# so just calling stty instead...
qx(stty -F /dev/ttyUSB0 115200);

my $PortObj = new Device::SerialPort ("/dev/ttyUSB0");


$PortObj->baudrate(115200);
$PortObj->parity("none");
$PortObj->databits(8);
$PortObj->stopbits(1);
$PortObj->handshake("none");
$PortObj->datatype('raw');
if(not defined $PortObj->write_settings) {
	exit -1;
}



my $js = new Linux::Joystick(1);
my $event;

print "Joystick has " . $js->buttonCount() . " buttons ".
        "and " . $js->axisCount() . " axes.\n";

# blocking reads:
while( $event = $js->nextEvent ) {

	#print "Event type: " . $event->type . ", ";
	if($event->isButton) {
		exit 0;
	} elsif($event->isAxis) {
		
		if($event->axis == 1) {
			my $value = int((($event->axisValue + 32767.0)/65535.0)*5000 + 1250);

			$PortObj->write(">A5$value");
		}elsif($event->axis == 2) {
			my $value = int((($event->axisValue + 32767.0)/65535.0)*5000 + 1250);

			$PortObj->write(">A6$value");
		}
		
		
	} else { # should never happen
		print "Unknown event " . $event->hexDump;
	}

}







$PortObj->close;
