use strict;
use warnings;
use Device::SerialPort qw( :PARAM :STAT 0.07 );
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



# Just a loop that drives servos 5 and 6 back and forth

my $value = 1250;
my $dir = 5;
while(1) {
	usleep(1000);
	$PortObj->write(">A5$value");
	$PortObj->write(">A6$value");
	$value += $dir;
	if(($value >= 6000) || ($value <= 1250)) {
		$dir = -$dir;
	}
}



$PortObj->close;
