#pragma once
#include <string>
#include <vector>

struct AVInputFormat;
struct AVFormatContext;
class DeviveReader
{
public:
	DeviveReader();

	~DeviveReader();


	void startRead(std::string camera);

private:
	static void cameraReadFunc(std::string camera, DeviveReader* dr);

	const AVInputFormat* listExistDevices(std::vector<std::string>& devs);
	
	AVFormatContext* openDeviceByName(const std::string& devName, const AVInputFormat* iptFmt);

	void readInFormatContext(AVFormatContext* ifmtCtx);




private:
	int m_width{640};
	int m_height{480};

};

