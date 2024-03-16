#include "DeviveReader.h"
#include <thread>


#include "global.h"


using namespace std;
DeviveReader::DeviveReader()
{
	av_log_set_level(AV_LOG_VERBOSE);
}

DeviveReader::~DeviveReader()
{
}

void DeviveReader::startRead(std::string camera)
{
	std::thread* t = new std::thread(cameraReadFunc,camera,this);
}

void DeviveReader::cameraReadFunc(std::string camera, DeviveReader* dr)
{
	std::vector<std::string> devices;
	const AVInputFormat* iptfmt = dr->listExistDevices(devices);

	AVFormatContext* ifmtCtx = dr->openDeviceByName(devices[0], iptfmt);

	dr->readInFormatContext(ifmtCtx);





}

const AVInputFormat* DeviveReader::listExistDevices(std::vector<std::string>& devs)
{
	avdevice_register_all();

	int ret = 0;
	const AVInputFormat* iptfmt = av_find_input_format("dshow");
	if (!iptfmt)
	{
		av_log(NULL,AV_LOG_ERROR, "av_find_input_format");
		return nullptr;
	}

	AVDeviceInfoList* devices = new AVDeviceInfoList;

	ret = avdevice_list_input_sources(iptfmt,NULL,NULL,&devices);
	if (ret <= 0)
	{
		av_log(NULL, AV_LOG_ERROR, "avdevice_list_input_sources");
		return nullptr;
	}

	std::vector<std::string> deviceName;
	for (int i = 0; i < devices->nb_devices; i++)
	{
		deviceName.emplace_back(devices->devices[i]->device_description);
		av_log(NULL,AV_LOG_INFO,"Devive: %s",devices->devices[i]->device_description);
	}

end:
	if (iptfmt)
	{
		delete iptfmt;
	}
	devs.swap(deviceName);

	return iptfmt;

}

AVFormatContext* DeviveReader::openDeviceByName(const std::string& devName, const AVInputFormat* iptFmt)
{
	AVFormatContext* ifmtCtx = avformat_alloc_context();
	if (!ifmtCtx)
	{
		av_log(NULL, AV_LOG_ERROR, "avformat_alloc_context");
		return nullptr;
	}

	AVDictionary* dict = NULL;
	string vs = to_string(m_width) + "*" + to_string(m_height);
	av_dict_set(&dict, "rtbufsize", "1820000", 0);
	av_dict_set(&dict, "video_size", vs.c_str(), 0);
	av_dict_set(&dict, "pixel_format", "yuyv422", 0);
	av_dict_set(&dict, "framerate", "30", 0);

	std::string dName = "video=" + devName;
	int ret = avformat_open_input(&ifmtCtx, dName.c_str(), iptFmt,&dict);
	if (ret != 0)
	{
		av_log(NULL, AV_LOG_ERROR, "avformat_open_input:%s",av_err2str(ret));
		return nullptr;
	}
	return ifmtCtx;
}

void DeviveReader::readInFormatContext(AVFormatContext* ifmtCtx)
{
	if (!ifmtCtx) {
		return;
	}

	AVPacket* packet = av_packet_alloc();
	if (!packet)
	{
		av_log(NULL, AV_LOG_ERROR, "av_packet_alloc");
		return;
	}
	av_init_packet(packet);

	int ret = 0;
	while (true)
	{
		ret = av_read_frame(ifmtCtx, packet);
		if (ret != 0) {

			av_log(NULL, AV_LOG_ERROR, "av_read_frame");
			return;
		}



	}
	














}
