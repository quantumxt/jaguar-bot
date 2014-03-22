#ifndef _kangaroo_hpp
#define _kangaroo_hpp

#include <ros/ros.h>
#include <trajectory_msgs/JointTrajectory.h>
#include <sensor_msgs/JointState.h>
#include <stdint.h>
#include <boost/thread.hpp>

namespace kangaroo
{

class kangaroo
{
public:
	kangaroo( ros::NodeHandle &_nh, ros::NodeHandle &_nh_priv );
	//~kangaroo();
	bool open( );
	void close( );
	bool start( );
	void stop( );
	bool send_start_signals(uint8_t address);
	int get_speed( uint8_t address, char channel, bool& ok );
	int get_position( unsigned char address, char channel, bool& ok );
	bool send_get_request( uint8_t address, char channel, uint8_t desired_parameter);
	void poll_kangaroo( unsigned char address );
	bool read_the_rest(unsigned char address);
	void read_thread();

private:
	bool is_open( ) const;
	void JointTrajCB( const trajectory_msgs::JointTrajectoryPtr &msg );
	//bool set_ch1( double speed, unsigned char address);
	//bool set_ch2( double speed, unsigned char address);

	int un_bitpack_number( uint8_t* data, size_t num_of_bytes );
	int evaluate_kangaroo_response( uint8_t address, uint8_t* header, uint8_t* data, bool& ok);
	void handle_errors(uint8_t address, int error_code);
	int read_from_serial(uint8_t address, char channel, bool& ok);
	uint8_t read_one_byte(bool& ok);
	bool set_channel_speed(double speed, unsigned char address, char channel);

	std::string port;
	std::string ch1_joint_name;
	std::string ch2_joint_name;
	int fd;

	ros::NodeHandle nh;
	ros::NodeHandle nh_priv;
	ros::Subscriber joint_traj_sub;
	boost::mutex io_mutex;

	ros::Publisher joint_state_pub;

	size_t bitpackNumber(uint8_t* buffer, int32_t number);
	uint16_t crc14(const uint8_t* data, size_t length);
	size_t writeKangarooCommand(uint8_t address, uint8_t command, const uint8_t* data, uint8_t length, uint8_t* buffer);
	size_t writeKangarooPositionCommand(uint8_t address, char channel, int32_t position, int32_t speedLimit, uint8_t* buffer);
	size_t writeKangarooSpeedCommand(uint8_t address, char channel, int32_t speed, uint8_t* buffer);
	size_t writeKangarooStartCommand(uint8_t address, char channel, uint8_t* buffer);
	size_t writeKangarooGetCommand(uint8_t address, char channel, char parameter, uint8_t* buffer);
};

}

#endif /* _kangaroo_hpp */

