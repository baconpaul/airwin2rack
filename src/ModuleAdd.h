#include "autogen_airwin/Chamber.h"
int Chamber_unused = AW2RModule::registerAirwindow({"Chamber", "Reverbs", airwin2rack::Chamber::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber::Chamber>(0); }});
#include "autogen_airwin/Galactic.h"
int Galactic_unused = AW2RModule::registerAirwindow({"Galactic", "Reverbs", airwin2rack::Galactic::kNumParameters, []() { return std::make_unique<airwin2rack::Galactic::Galactic>(0); }});
#include "autogen_airwin/MatrixVerb.h"
int MatrixVerb_unused = AW2RModule::registerAirwindow({"MatrixVerb", "Reverbs", airwin2rack::MatrixVerb::kNumParameters, []() { return std::make_unique<airwin2rack::MatrixVerb::MatrixVerb>(0); }});
#include "autogen_airwin/BrightAmbience.h"
int BrightAmbience_unused = AW2RModule::registerAirwindow({"BrightAmbience", "Ambience", airwin2rack::BrightAmbience::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience::BrightAmbience>(0); }});
#include "autogen_airwin/BrightAmbience2.h"
int BrightAmbience2_unused = AW2RModule::registerAirwindow({"BrightAmbience2", "Ambience", airwin2rack::BrightAmbience2::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience2::BrightAmbience2>(0); }});
#include "autogen_airwin/BrightAmbience3.h"
int BrightAmbience3_unused = AW2RModule::registerAirwindow({"BrightAmbience3", "Ambience", airwin2rack::BrightAmbience3::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience3::BrightAmbience3>(0); }});
#include "autogen_airwin/CrunchyGrooveWear.h"
int CrunchyGrooveWear_unused = AW2RModule::registerAirwindow({"CrunchyGrooveWear", "Tape and Vinyl", airwin2rack::CrunchyGrooveWear::kNumParameters, []() { return std::make_unique<airwin2rack::CrunchyGrooveWear::CrunchyGrooveWear>(0); }});
#include "autogen_airwin/IronOxide5.h"
int IronOxide5_unused = AW2RModule::registerAirwindow({"IronOxide5", "Tape and Vinyl", airwin2rack::IronOxide5::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxide5::IronOxide5>(0); }});
#include "autogen_airwin/IronOxideClassic.h"
int IronOxideClassic_unused = AW2RModule::registerAirwindow({"IronOxideClassic", "Tape and Vinyl", airwin2rack::IronOxideClassic::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic::IronOxideClassic>(0); }});
#include "autogen_airwin/IronOxideClassic2.h"
int IronOxideClassic2_unused = AW2RModule::registerAirwindow({"IronOxideClassic2", "Tape and Vinyl", airwin2rack::IronOxideClassic2::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic2::IronOxideClassic2>(0); }});
#include "autogen_airwin/FromTape.h"
int FromTape_unused = AW2RModule::registerAirwindow({"FromTape", "Tape and Vinyl", airwin2rack::FromTape::kNumParameters, []() { return std::make_unique<airwin2rack::FromTape::FromTape>(0); }});
#include "autogen_airwin/Tape.h"
int Tape_unused = AW2RModule::registerAirwindow({"Tape", "Tape and Vinyl", airwin2rack::Tape::kNumParameters, []() { return std::make_unique<airwin2rack::Tape::Tape>(0); }});
#include "autogen_airwin/TapeDelay.h"
int TapeDelay_unused = AW2RModule::registerAirwindow({"TapeDelay", "Tape and Vinyl", airwin2rack::TapeDelay::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDelay::TapeDelay>(0); }});
#include "autogen_airwin/TapeDelay2.h"
int TapeDelay2_unused = AW2RModule::registerAirwindow({"TapeDelay2", "Tape and Vinyl", airwin2rack::TapeDelay2::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDelay2::TapeDelay2>(0); }});
#include "autogen_airwin/TapeDither.h"
int TapeDither_unused = AW2RModule::registerAirwindow({"TapeDither", "Tape and Vinyl", airwin2rack::TapeDither::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDither::TapeDither>(0); }});
#include "autogen_airwin/TapeDust.h"
int TapeDust_unused = AW2RModule::registerAirwindow({"TapeDust", "Tape and Vinyl", airwin2rack::TapeDust::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDust::TapeDust>(0); }});
#include "autogen_airwin/TapeFat.h"
int TapeFat_unused = AW2RModule::registerAirwindow({"TapeFat", "Tape and Vinyl", airwin2rack::TapeFat::kNumParameters, []() { return std::make_unique<airwin2rack::TapeFat::TapeFat>(0); }});
#include "autogen_airwin/ToTape5.h"
int ToTape5_unused = AW2RModule::registerAirwindow({"ToTape5", "Tape and Vinyl", airwin2rack::ToTape5::kNumParameters, []() { return std::make_unique<airwin2rack::ToTape5::ToTape5>(0); }});
#include "autogen_airwin/ToTape6.h"
int ToTape6_unused = AW2RModule::registerAirwindow({"ToTape6", "Tape and Vinyl", airwin2rack::ToTape6::kNumParameters, []() { return std::make_unique<airwin2rack::ToTape6::ToTape6>(0); }});
#include "autogen_airwin/ToVinyl4.h"
int ToVinyl4_unused = AW2RModule::registerAirwindow({"ToVinyl4", "Tape and Vinyl", airwin2rack::ToVinyl4::kNumParameters, []() { return std::make_unique<airwin2rack::ToVinyl4::ToVinyl4>(0); }});
#include "autogen_airwin/Console4Buss.h"
int Console4Buss_unused = AW2RModule::registerAirwindow({"Console4Buss", "Console", airwin2rack::Console4Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console4Buss::Console4Buss>(0); }});
#include "autogen_airwin/Console4Channel.h"
int Console4Channel_unused = AW2RModule::registerAirwindow({"Console4Channel", "Console", airwin2rack::Console4Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console4Channel::Console4Channel>(0); }});
#include "autogen_airwin/Console5Buss.h"
int Console5Buss_unused = AW2RModule::registerAirwindow({"Console5Buss", "Console", airwin2rack::Console5Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console5Buss::Console5Buss>(0); }});
#include "autogen_airwin/Console5Channel.h"
int Console5Channel_unused = AW2RModule::registerAirwindow({"Console5Channel", "Console", airwin2rack::Console5Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console5Channel::Console5Channel>(0); }});
#include "autogen_airwin/Console5DarkCh.h"
int Console5DarkCh_unused = AW2RModule::registerAirwindow({"Console5DarkCh", "Console", airwin2rack::Console5DarkCh::kNumParameters, []() { return std::make_unique<airwin2rack::Console5DarkCh::Console5DarkCh>(0); }});
#include "autogen_airwin/Console6Buss.h"
int Console6Buss_unused = AW2RModule::registerAirwindow({"Console6Buss", "Console", airwin2rack::Console6Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console6Buss::Console6Buss>(0); }});
#include "autogen_airwin/Console6Channel.h"
int Console6Channel_unused = AW2RModule::registerAirwindow({"Console6Channel", "Console", airwin2rack::Console6Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console6Channel::Console6Channel>(0); }});
#include "autogen_airwin/Console7Buss.h"
int Console7Buss_unused = AW2RModule::registerAirwindow({"Console7Buss", "Console", airwin2rack::Console7Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Buss::Console7Buss>(0); }});
#include "autogen_airwin/Console7Cascade.h"
int Console7Cascade_unused = AW2RModule::registerAirwindow({"Console7Cascade", "Console", airwin2rack::Console7Cascade::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Cascade::Console7Cascade>(0); }});
#include "autogen_airwin/Console7Channel.h"
int Console7Channel_unused = AW2RModule::registerAirwindow({"Console7Channel", "Console", airwin2rack::Console7Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Channel::Console7Channel>(0); }});
#include "autogen_airwin/Console7Crunch.h"
int Console7Crunch_unused = AW2RModule::registerAirwindow({"Console7Crunch", "Console", airwin2rack::Console7Crunch::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Crunch::Console7Crunch>(0); }});
#include "autogen_airwin/Console8BussHype.h"
int Console8BussHype_unused = AW2RModule::registerAirwindow({"Console8BussHype", "Console", airwin2rack::Console8BussHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussHype::Console8BussHype>(0); }});
#include "autogen_airwin/Console8BussIn.h"
int Console8BussIn_unused = AW2RModule::registerAirwindow({"Console8BussIn", "Console", airwin2rack::Console8BussIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussIn::Console8BussIn>(0); }});
#include "autogen_airwin/Console8BussOut.h"
int Console8BussOut_unused = AW2RModule::registerAirwindow({"Console8BussOut", "Console", airwin2rack::Console8BussOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussOut::Console8BussOut>(0); }});
#include "autogen_airwin/Console8ChannelHype.h"
int Console8ChannelHype_unused = AW2RModule::registerAirwindow({"Console8ChannelHype", "Console", airwin2rack::Console8ChannelHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelHype::Console8ChannelHype>(0); }});
#include "autogen_airwin/Console8ChannelIn.h"
int Console8ChannelIn_unused = AW2RModule::registerAirwindow({"Console8ChannelIn", "Console", airwin2rack::Console8ChannelIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelIn::Console8ChannelIn>(0); }});
#include "autogen_airwin/Console8ChannelOut.h"
int Console8ChannelOut_unused = AW2RModule::registerAirwindow({"Console8ChannelOut", "Console", airwin2rack::Console8ChannelOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelOut::Console8ChannelOut>(0); }});
#include "autogen_airwin/Console8SubHype.h"
int Console8SubHype_unused = AW2RModule::registerAirwindow({"Console8SubHype", "Console", airwin2rack::Console8SubHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubHype::Console8SubHype>(0); }});
#include "autogen_airwin/Console8SubIn.h"
int Console8SubIn_unused = AW2RModule::registerAirwindow({"Console8SubIn", "Console", airwin2rack::Console8SubIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubIn::Console8SubIn>(0); }});
#include "autogen_airwin/Console8SubOut.h"
int Console8SubOut_unused = AW2RModule::registerAirwindow({"Console8SubOut", "Console", airwin2rack::Console8SubOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubOut::Console8SubOut>(0); }});
#include "autogen_airwin/PurestAir.h"
int PurestAir_unused = AW2RModule::registerAirwindow({"PurestAir", "Purest Variations", airwin2rack::PurestAir::kNumParameters, []() { return std::make_unique<airwin2rack::PurestAir::PurestAir>(0); }});
#include "autogen_airwin/PurestConsole2Buss.h"
int PurestConsole2Buss_unused = AW2RModule::registerAirwindow({"PurestConsole2Buss", "Purest Variations", airwin2rack::PurestConsole2Buss::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole2Buss::PurestConsole2Buss>(0); }});
#include "autogen_airwin/PurestConsole2Channel.h"
int PurestConsole2Channel_unused = AW2RModule::registerAirwindow({"PurestConsole2Channel", "Purest Variations", airwin2rack::PurestConsole2Channel::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole2Channel::PurestConsole2Channel>(0); }});
#include "autogen_airwin/PurestConsoleBuss.h"
int PurestConsoleBuss_unused = AW2RModule::registerAirwindow({"PurestConsoleBuss", "Purest Variations", airwin2rack::PurestConsoleBuss::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsoleBuss::PurestConsoleBuss>(0); }});
#include "autogen_airwin/PurestConsoleChannel.h"
int PurestConsoleChannel_unused = AW2RModule::registerAirwindow({"PurestConsoleChannel", "Purest Variations", airwin2rack::PurestConsoleChannel::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsoleChannel::PurestConsoleChannel>(0); }});
#include "autogen_airwin/PurestDrive.h"
int PurestDrive_unused = AW2RModule::registerAirwindow({"PurestDrive", "Purest Variations", airwin2rack::PurestDrive::kNumParameters, []() { return std::make_unique<airwin2rack::PurestDrive::PurestDrive>(0); }});
#include "autogen_airwin/PurestEcho.h"
int PurestEcho_unused = AW2RModule::registerAirwindow({"PurestEcho", "Purest Variations", airwin2rack::PurestEcho::kNumParameters, []() { return std::make_unique<airwin2rack::PurestEcho::PurestEcho>(0); }});
#include "autogen_airwin/PurestFade.h"
int PurestFade_unused = AW2RModule::registerAirwindow({"PurestFade", "Purest Variations", airwin2rack::PurestFade::kNumParameters, []() { return std::make_unique<airwin2rack::PurestFade::PurestFade>(0); }});
#include "autogen_airwin/PurestGain.h"
int PurestGain_unused = AW2RModule::registerAirwindow({"PurestGain", "Purest Variations", airwin2rack::PurestGain::kNumParameters, []() { return std::make_unique<airwin2rack::PurestGain::PurestGain>(0); }});
#include "autogen_airwin/PurestSquish.h"
int PurestSquish_unused = AW2RModule::registerAirwindow({"PurestSquish", "Purest Variations", airwin2rack::PurestSquish::kNumParameters, []() { return std::make_unique<airwin2rack::PurestSquish::PurestSquish>(0); }});
#include "autogen_airwin/PurestWarm.h"
int PurestWarm_unused = AW2RModule::registerAirwindow({"PurestWarm", "Purest Variations", airwin2rack::PurestWarm::kNumParameters, []() { return std::make_unique<airwin2rack::PurestWarm::PurestWarm>(0); }});
#include "autogen_airwin/PurestWarm2.h"
int PurestWarm2_unused = AW2RModule::registerAirwindow({"PurestWarm2", "Purest Variations", airwin2rack::PurestWarm2::kNumParameters, []() { return std::make_unique<airwin2rack::PurestWarm2::PurestWarm2>(0); }});
#include "autogen_airwin/EQ.h"
int EQ_unused = AW2RModule::registerAirwindow({"EQ", "EQs", airwin2rack::EQ::kNumParameters, []() { return std::make_unique<airwin2rack::EQ::EQ>(0); }});
#include "autogen_airwin/ResEQ.h"
int ResEQ_unused = AW2RModule::registerAirwindow({"ResEQ", "EQs", airwin2rack::ResEQ::kNumParameters, []() { return std::make_unique<airwin2rack::ResEQ::ResEQ>(0); }});
#include "autogen_airwin/XBandpass.h"
int XBandpass_unused = AW2RModule::registerAirwindow({"XBandpass", "XYZ Filters", airwin2rack::XBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::XBandpass::XBandpass>(0); }});
#include "autogen_airwin/XHighpass.h"
int XHighpass_unused = AW2RModule::registerAirwindow({"XHighpass", "XYZ Filters", airwin2rack::XHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::XHighpass::XHighpass>(0); }});
#include "autogen_airwin/XLowpass.h"
int XLowpass_unused = AW2RModule::registerAirwindow({"XLowpass", "XYZ Filters", airwin2rack::XLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::XLowpass::XLowpass>(0); }});
#include "autogen_airwin/XNotch.h"
int XNotch_unused = AW2RModule::registerAirwindow({"XNotch", "XYZ Filters", airwin2rack::XNotch::kNumParameters, []() { return std::make_unique<airwin2rack::XNotch::XNotch>(0); }});
#include "autogen_airwin/XRegion.h"
int XRegion_unused = AW2RModule::registerAirwindow({"XRegion", "XYZ Filters", airwin2rack::XRegion::kNumParameters, []() { return std::make_unique<airwin2rack::XRegion::XRegion>(0); }});
#include "autogen_airwin/YBandpass.h"
int YBandpass_unused = AW2RModule::registerAirwindow({"YBandpass", "XYZ Filters", airwin2rack::YBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::YBandpass::YBandpass>(0); }});
#include "autogen_airwin/YHighpass.h"
int YHighpass_unused = AW2RModule::registerAirwindow({"YHighpass", "XYZ Filters", airwin2rack::YHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::YHighpass::YHighpass>(0); }});
#include "autogen_airwin/YLowpass.h"
int YLowpass_unused = AW2RModule::registerAirwindow({"YLowpass", "XYZ Filters", airwin2rack::YLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::YLowpass::YLowpass>(0); }});
#include "autogen_airwin/YNotch.h"
int YNotch_unused = AW2RModule::registerAirwindow({"YNotch", "XYZ Filters", airwin2rack::YNotch::kNumParameters, []() { return std::make_unique<airwin2rack::YNotch::YNotch>(0); }});
#include "autogen_airwin/ZBandpass.h"
int ZBandpass_unused = AW2RModule::registerAirwindow({"ZBandpass", "XYZ Filters", airwin2rack::ZBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZBandpass::ZBandpass>(0); }});
#include "autogen_airwin/ZBandpass2.h"
int ZBandpass2_unused = AW2RModule::registerAirwindow({"ZBandpass2", "XYZ Filters", airwin2rack::ZBandpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZBandpass2::ZBandpass2>(0); }});
#include "autogen_airwin/ZHighpass.h"
int ZHighpass_unused = AW2RModule::registerAirwindow({"ZHighpass", "XYZ Filters", airwin2rack::ZHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZHighpass::ZHighpass>(0); }});
#include "autogen_airwin/ZHighpass2.h"
int ZHighpass2_unused = AW2RModule::registerAirwindow({"ZHighpass2", "XYZ Filters", airwin2rack::ZHighpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZHighpass2::ZHighpass2>(0); }});
#include "autogen_airwin/ZLowpass.h"
int ZLowpass_unused = AW2RModule::registerAirwindow({"ZLowpass", "XYZ Filters", airwin2rack::ZLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZLowpass::ZLowpass>(0); }});
#include "autogen_airwin/ZLowpass2.h"
int ZLowpass2_unused = AW2RModule::registerAirwindow({"ZLowpass2", "XYZ Filters", airwin2rack::ZLowpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZLowpass2::ZLowpass2>(0); }});
#include "autogen_airwin/ZNotch.h"
int ZNotch_unused = AW2RModule::registerAirwindow({"ZNotch", "XYZ Filters", airwin2rack::ZNotch::kNumParameters, []() { return std::make_unique<airwin2rack::ZNotch::ZNotch>(0); }});
#include "autogen_airwin/ZNotch2.h"
int ZNotch2_unused = AW2RModule::registerAirwindow({"ZNotch2", "XYZ Filters", airwin2rack::ZNotch2::kNumParameters, []() { return std::make_unique<airwin2rack::ZNotch2::ZNotch2>(0); }});
#include "autogen_airwin/ZRegion.h"
int ZRegion_unused = AW2RModule::registerAirwindow({"ZRegion", "XYZ Filters", airwin2rack::ZRegion::kNumParameters, []() { return std::make_unique<airwin2rack::ZRegion::ZRegion>(0); }});
#include "autogen_airwin/ZRegion2.h"
int ZRegion2_unused = AW2RModule::registerAirwindow({"ZRegion2", "XYZ Filters", airwin2rack::ZRegion2::kNumParameters, []() { return std::make_unique<airwin2rack::ZRegion2::ZRegion2>(0); }});
#include "autogen_airwin/ADClip7.h"
int ADClip7_unused = AW2RModule::registerAirwindow({"ADClip7", "Unclassified", airwin2rack::ADClip7::kNumParameters, []() { return std::make_unique<airwin2rack::ADClip7::ADClip7>(0); }});
#include "autogen_airwin/ADT.h"
int ADT_unused = AW2RModule::registerAirwindow({"ADT", "Unclassified", airwin2rack::ADT::kNumParameters, []() { return std::make_unique<airwin2rack::ADT::ADT>(0); }});
#include "autogen_airwin/AQuickVoiceClip.h"
int AQuickVoiceClip_unused = AW2RModule::registerAirwindow({"AQuickVoiceClip", "Unclassified", airwin2rack::AQuickVoiceClip::kNumParameters, []() { return std::make_unique<airwin2rack::AQuickVoiceClip::AQuickVoiceClip>(0); }});
#include "autogen_airwin/Acceleration.h"
int Acceleration_unused = AW2RModule::registerAirwindow({"Acceleration", "Unclassified", airwin2rack::Acceleration::kNumParameters, []() { return std::make_unique<airwin2rack::Acceleration::Acceleration>(0); }});
#include "autogen_airwin/Acceleration2.h"
int Acceleration2_unused = AW2RModule::registerAirwindow({"Acceleration2", "Unclassified", airwin2rack::Acceleration2::kNumParameters, []() { return std::make_unique<airwin2rack::Acceleration2::Acceleration2>(0); }});
#include "autogen_airwin/Air.h"
int Air_unused = AW2RModule::registerAirwindow({"Air", "Unclassified", airwin2rack::Air::kNumParameters, []() { return std::make_unique<airwin2rack::Air::Air>(0); }});
#include "autogen_airwin/Air2.h"
int Air2_unused = AW2RModule::registerAirwindow({"Air2", "Unclassified", airwin2rack::Air2::kNumParameters, []() { return std::make_unique<airwin2rack::Air2::Air2>(0); }});
#include "autogen_airwin/Apicolypse.h"
int Apicolypse_unused = AW2RModule::registerAirwindow({"Apicolypse", "Unclassified", airwin2rack::Apicolypse::kNumParameters, []() { return std::make_unique<airwin2rack::Apicolypse::Apicolypse>(0); }});
#include "autogen_airwin/AtmosphereBuss.h"
int AtmosphereBuss_unused = AW2RModule::registerAirwindow({"AtmosphereBuss", "Unclassified", airwin2rack::AtmosphereBuss::kNumParameters, []() { return std::make_unique<airwin2rack::AtmosphereBuss::AtmosphereBuss>(0); }});
#include "autogen_airwin/AtmosphereChannel.h"
int AtmosphereChannel_unused = AW2RModule::registerAirwindow({"AtmosphereChannel", "Unclassified", airwin2rack::AtmosphereChannel::kNumParameters, []() { return std::make_unique<airwin2rack::AtmosphereChannel::AtmosphereChannel>(0); }});
#include "autogen_airwin/Aura.h"
int Aura_unused = AW2RModule::registerAirwindow({"Aura", "Unclassified", airwin2rack::Aura::kNumParameters, []() { return std::make_unique<airwin2rack::Aura::Aura>(0); }});
#include "autogen_airwin/AutoPan.h"
int AutoPan_unused = AW2RModule::registerAirwindow({"AutoPan", "Unclassified", airwin2rack::AutoPan::kNumParameters, []() { return std::make_unique<airwin2rack::AutoPan::AutoPan>(0); }});
#include "autogen_airwin/AverMatrix.h"
int AverMatrix_unused = AW2RModule::registerAirwindow({"AverMatrix", "Unclassified", airwin2rack::AverMatrix::kNumParameters, []() { return std::make_unique<airwin2rack::AverMatrix::AverMatrix>(0); }});
#include "autogen_airwin/Average.h"
int Average_unused = AW2RModule::registerAirwindow({"Average", "Unclassified", airwin2rack::Average::kNumParameters, []() { return std::make_unique<airwin2rack::Average::Average>(0); }});
#include "autogen_airwin/Balanced.h"
int Balanced_unused = AW2RModule::registerAirwindow({"Balanced", "Unclassified", airwin2rack::Balanced::kNumParameters, []() { return std::make_unique<airwin2rack::Balanced::Balanced>(0); }});
#include "autogen_airwin/BassAmp.h"
int BassAmp_unused = AW2RModule::registerAirwindow({"BassAmp", "Unclassified", airwin2rack::BassAmp::kNumParameters, []() { return std::make_unique<airwin2rack::BassAmp::BassAmp>(0); }});
#include "autogen_airwin/BassDrive.h"
int BassDrive_unused = AW2RModule::registerAirwindow({"BassDrive", "Unclassified", airwin2rack::BassDrive::kNumParameters, []() { return std::make_unique<airwin2rack::BassDrive::BassDrive>(0); }});
#include "autogen_airwin/BassKit.h"
int BassKit_unused = AW2RModule::registerAirwindow({"BassKit", "Unclassified", airwin2rack::BassKit::kNumParameters, []() { return std::make_unique<airwin2rack::BassKit::BassKit>(0); }});
#include "autogen_airwin/Baxandall.h"
int Baxandall_unused = AW2RModule::registerAirwindow({"Baxandall", "Unclassified", airwin2rack::Baxandall::kNumParameters, []() { return std::make_unique<airwin2rack::Baxandall::Baxandall>(0); }});
#include "autogen_airwin/Baxandall2.h"
int Baxandall2_unused = AW2RModule::registerAirwindow({"Baxandall2", "Unclassified", airwin2rack::Baxandall2::kNumParameters, []() { return std::make_unique<airwin2rack::Baxandall2::Baxandall2>(0); }});
#include "autogen_airwin/Beam.h"
int Beam_unused = AW2RModule::registerAirwindow({"Beam", "Unclassified", airwin2rack::Beam::kNumParameters, []() { return std::make_unique<airwin2rack::Beam::Beam>(0); }});
#include "autogen_airwin/BigAmp.h"
int BigAmp_unused = AW2RModule::registerAirwindow({"BigAmp", "Unclassified", airwin2rack::BigAmp::kNumParameters, []() { return std::make_unique<airwin2rack::BigAmp::BigAmp>(0); }});
#include "autogen_airwin/Biquad.h"
int Biquad_unused = AW2RModule::registerAirwindow({"Biquad", "Unclassified", airwin2rack::Biquad::kNumParameters, []() { return std::make_unique<airwin2rack::Biquad::Biquad>(0); }});
#include "autogen_airwin/Biquad2.h"
int Biquad2_unused = AW2RModule::registerAirwindow({"Biquad2", "Unclassified", airwin2rack::Biquad2::kNumParameters, []() { return std::make_unique<airwin2rack::Biquad2::Biquad2>(0); }});
#include "autogen_airwin/BiquadDouble.h"
int BiquadDouble_unused = AW2RModule::registerAirwindow({"BiquadDouble", "Unclassified", airwin2rack::BiquadDouble::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadDouble::BiquadDouble>(0); }});
#include "autogen_airwin/BiquadOneHalf.h"
int BiquadOneHalf_unused = AW2RModule::registerAirwindow({"BiquadOneHalf", "Unclassified", airwin2rack::BiquadOneHalf::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadOneHalf::BiquadOneHalf>(0); }});
#include "autogen_airwin/BiquadPlus.h"
int BiquadPlus_unused = AW2RModule::registerAirwindow({"BiquadPlus", "Unclassified", airwin2rack::BiquadPlus::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadPlus::BiquadPlus>(0); }});
#include "autogen_airwin/BiquadTriple.h"
int BiquadTriple_unused = AW2RModule::registerAirwindow({"BiquadTriple", "Unclassified", airwin2rack::BiquadTriple::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadTriple::BiquadTriple>(0); }});
#include "autogen_airwin/BitGlitter.h"
int BitGlitter_unused = AW2RModule::registerAirwindow({"BitGlitter", "Unclassified", airwin2rack::BitGlitter::kNumParameters, []() { return std::make_unique<airwin2rack::BitGlitter::BitGlitter>(0); }});
#include "autogen_airwin/BitShiftGain.h"
int BitShiftGain_unused = AW2RModule::registerAirwindow({"BitShiftGain", "Unclassified", airwin2rack::BitShiftGain::kNumParameters, []() { return std::make_unique<airwin2rack::BitShiftGain::BitShiftGain>(0); }});
#include "autogen_airwin/Bite.h"
int Bite_unused = AW2RModule::registerAirwindow({"Bite", "Unclassified", airwin2rack::Bite::kNumParameters, []() { return std::make_unique<airwin2rack::Bite::Bite>(0); }});
#include "autogen_airwin/BlockParty.h"
int BlockParty_unused = AW2RModule::registerAirwindow({"BlockParty", "Unclassified", airwin2rack::BlockParty::kNumParameters, []() { return std::make_unique<airwin2rack::BlockParty::BlockParty>(0); }});
#include "autogen_airwin/BrassRider.h"
int BrassRider_unused = AW2RModule::registerAirwindow({"BrassRider", "Unclassified", airwin2rack::BrassRider::kNumParameters, []() { return std::make_unique<airwin2rack::BrassRider::BrassRider>(0); }});
#include "autogen_airwin/BuildATPDF.h"
int BuildATPDF_unused = AW2RModule::registerAirwindow({"BuildATPDF", "Unclassified", airwin2rack::BuildATPDF::kNumParameters, []() { return std::make_unique<airwin2rack::BuildATPDF::BuildATPDF>(0); }});
#include "autogen_airwin/BussColors4.h"
int BussColors4_unused = AW2RModule::registerAirwindow({"BussColors4", "Unclassified", airwin2rack::BussColors4::kNumParameters, []() { return std::make_unique<airwin2rack::BussColors4::BussColors4>(0); }});
#include "autogen_airwin/ButterComp.h"
int ButterComp_unused = AW2RModule::registerAirwindow({"ButterComp", "Unclassified", airwin2rack::ButterComp::kNumParameters, []() { return std::make_unique<airwin2rack::ButterComp::ButterComp>(0); }});
#include "autogen_airwin/ButterComp2.h"
int ButterComp2_unused = AW2RModule::registerAirwindow({"ButterComp2", "Unclassified", airwin2rack::ButterComp2::kNumParameters, []() { return std::make_unique<airwin2rack::ButterComp2::ButterComp2>(0); }});
#include "autogen_airwin/C5RawBuss.h"
int C5RawBuss_unused = AW2RModule::registerAirwindow({"C5RawBuss", "Unclassified", airwin2rack::C5RawBuss::kNumParameters, []() { return std::make_unique<airwin2rack::C5RawBuss::C5RawBuss>(0); }});
#include "autogen_airwin/C5RawChannel.h"
int C5RawChannel_unused = AW2RModule::registerAirwindow({"C5RawChannel", "Unclassified", airwin2rack::C5RawChannel::kNumParameters, []() { return std::make_unique<airwin2rack::C5RawChannel::C5RawChannel>(0); }});
#include "autogen_airwin/CStrip.h"
int CStrip_unused = AW2RModule::registerAirwindow({"CStrip", "Unclassified", airwin2rack::CStrip::kNumParameters, []() { return std::make_unique<airwin2rack::CStrip::CStrip>(0); }});
#include "autogen_airwin/Cabs.h"
int Cabs_unused = AW2RModule::registerAirwindow({"Cabs", "Unclassified", airwin2rack::Cabs::kNumParameters, []() { return std::make_unique<airwin2rack::Cabs::Cabs>(0); }});
#include "autogen_airwin/Calibre.h"
int Calibre_unused = AW2RModule::registerAirwindow({"Calibre", "Unclassified", airwin2rack::Calibre::kNumParameters, []() { return std::make_unique<airwin2rack::Calibre::Calibre>(0); }});
#include "autogen_airwin/Capacitor.h"
int Capacitor_unused = AW2RModule::registerAirwindow({"Capacitor", "Unclassified", airwin2rack::Capacitor::kNumParameters, []() { return std::make_unique<airwin2rack::Capacitor::Capacitor>(0); }});
#include "autogen_airwin/Capacitor2.h"
int Capacitor2_unused = AW2RModule::registerAirwindow({"Capacitor2", "Unclassified", airwin2rack::Capacitor2::kNumParameters, []() { return std::make_unique<airwin2rack::Capacitor2::Capacitor2>(0); }});
#include "autogen_airwin/Chamber2.h"
int Chamber2_unused = AW2RModule::registerAirwindow({"Chamber2", "Unclassified", airwin2rack::Chamber2::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber2::Chamber2>(0); }});
#include "autogen_airwin/Channel4.h"
int Channel4_unused = AW2RModule::registerAirwindow({"Channel4", "Unclassified", airwin2rack::Channel4::kNumParameters, []() { return std::make_unique<airwin2rack::Channel4::Channel4>(0); }});
#include "autogen_airwin/Channel5.h"
int Channel5_unused = AW2RModule::registerAirwindow({"Channel5", "Unclassified", airwin2rack::Channel5::kNumParameters, []() { return std::make_unique<airwin2rack::Channel5::Channel5>(0); }});
#include "autogen_airwin/Channel6.h"
int Channel6_unused = AW2RModule::registerAirwindow({"Channel6", "Unclassified", airwin2rack::Channel6::kNumParameters, []() { return std::make_unique<airwin2rack::Channel6::Channel6>(0); }});
#include "autogen_airwin/Channel7.h"
int Channel7_unused = AW2RModule::registerAirwindow({"Channel7", "Unclassified", airwin2rack::Channel7::kNumParameters, []() { return std::make_unique<airwin2rack::Channel7::Channel7>(0); }});
#include "autogen_airwin/Channel8.h"
int Channel8_unused = AW2RModule::registerAirwindow({"Channel8", "Unclassified", airwin2rack::Channel8::kNumParameters, []() { return std::make_unique<airwin2rack::Channel8::Channel8>(0); }});
#include "autogen_airwin/Channel9.h"
int Channel9_unused = AW2RModule::registerAirwindow({"Channel9", "Unclassified", airwin2rack::Channel9::kNumParameters, []() { return std::make_unique<airwin2rack::Channel9::Channel9>(0); }});
#include "autogen_airwin/Chorus.h"
int Chorus_unused = AW2RModule::registerAirwindow({"Chorus", "Unclassified", airwin2rack::Chorus::kNumParameters, []() { return std::make_unique<airwin2rack::Chorus::Chorus>(0); }});
#include "autogen_airwin/ChorusEnsemble.h"
int ChorusEnsemble_unused = AW2RModule::registerAirwindow({"ChorusEnsemble", "Unclassified", airwin2rack::ChorusEnsemble::kNumParameters, []() { return std::make_unique<airwin2rack::ChorusEnsemble::ChorusEnsemble>(0); }});
#include "autogen_airwin/ChromeOxide.h"
int ChromeOxide_unused = AW2RModule::registerAirwindow({"ChromeOxide", "Unclassified", airwin2rack::ChromeOxide::kNumParameters, []() { return std::make_unique<airwin2rack::ChromeOxide::ChromeOxide>(0); }});
#include "autogen_airwin/Cider.h"
int Cider_unused = AW2RModule::registerAirwindow({"Cider", "Unclassified", airwin2rack::Cider::kNumParameters, []() { return std::make_unique<airwin2rack::Cider::Cider>(0); }});
#include "autogen_airwin/ClipOnly2.h"
int ClipOnly2_unused = AW2RModule::registerAirwindow({"ClipOnly2", "Unclassified", airwin2rack::ClipOnly2::kNumParameters, []() { return std::make_unique<airwin2rack::ClipOnly2::ClipOnly2>(0); }});
#include "autogen_airwin/ClipSoftly.h"
int ClipSoftly_unused = AW2RModule::registerAirwindow({"ClipSoftly", "Unclassified", airwin2rack::ClipSoftly::kNumParameters, []() { return std::make_unique<airwin2rack::ClipSoftly::ClipSoftly>(0); }});
#include "autogen_airwin/Coils.h"
int Coils_unused = AW2RModule::registerAirwindow({"Coils", "Unclassified", airwin2rack::Coils::kNumParameters, []() { return std::make_unique<airwin2rack::Coils::Coils>(0); }});
#include "autogen_airwin/Coils2.h"
int Coils2_unused = AW2RModule::registerAirwindow({"Coils2", "Unclassified", airwin2rack::Coils2::kNumParameters, []() { return std::make_unique<airwin2rack::Coils2::Coils2>(0); }});
#include "autogen_airwin/Cojones.h"
int Cojones_unused = AW2RModule::registerAirwindow({"Cojones", "Unclassified", airwin2rack::Cojones::kNumParameters, []() { return std::make_unique<airwin2rack::Cojones::Cojones>(0); }});
#include "autogen_airwin/Compresaturator.h"
int Compresaturator_unused = AW2RModule::registerAirwindow({"Compresaturator", "Unclassified", airwin2rack::Compresaturator::kNumParameters, []() { return std::make_unique<airwin2rack::Compresaturator::Compresaturator>(0); }});
#include "autogen_airwin/Crystal.h"
int Crystal_unused = AW2RModule::registerAirwindow({"Crystal", "Unclassified", airwin2rack::Crystal::kNumParameters, []() { return std::make_unique<airwin2rack::Crystal::Crystal>(0); }});
#include "autogen_airwin/DCVoltage.h"
int DCVoltage_unused = AW2RModule::registerAirwindow({"DCVoltage", "Unclassified", airwin2rack::DCVoltage::kNumParameters, []() { return std::make_unique<airwin2rack::DCVoltage::DCVoltage>(0); }});
#include "autogen_airwin/Dark.h"
int Dark_unused = AW2RModule::registerAirwindow({"Dark", "Unclassified", airwin2rack::Dark::kNumParameters, []() { return std::make_unique<airwin2rack::Dark::Dark>(0); }});
#include "autogen_airwin/DarkNoise.h"
int DarkNoise_unused = AW2RModule::registerAirwindow({"DarkNoise", "Unclassified", airwin2rack::DarkNoise::kNumParameters, []() { return std::make_unique<airwin2rack::DarkNoise::DarkNoise>(0); }});
#include "autogen_airwin/DeBess.h"
int DeBess_unused = AW2RModule::registerAirwindow({"DeBess", "Unclassified", airwin2rack::DeBess::kNumParameters, []() { return std::make_unique<airwin2rack::DeBess::DeBess>(0); }});
#include "autogen_airwin/DeEss.h"
int DeEss_unused = AW2RModule::registerAirwindow({"DeEss", "Unclassified", airwin2rack::DeEss::kNumParameters, []() { return std::make_unique<airwin2rack::DeEss::DeEss>(0); }});
#include "autogen_airwin/DeHiss.h"
int DeHiss_unused = AW2RModule::registerAirwindow({"DeHiss", "Unclassified", airwin2rack::DeHiss::kNumParameters, []() { return std::make_unique<airwin2rack::DeHiss::DeHiss>(0); }});
#include "autogen_airwin/DeRez.h"
int DeRez_unused = AW2RModule::registerAirwindow({"DeRez", "Unclassified", airwin2rack::DeRez::kNumParameters, []() { return std::make_unique<airwin2rack::DeRez::DeRez>(0); }});
#include "autogen_airwin/DeRez2.h"
int DeRez2_unused = AW2RModule::registerAirwindow({"DeRez2", "Unclassified", airwin2rack::DeRez2::kNumParameters, []() { return std::make_unique<airwin2rack::DeRez2::DeRez2>(0); }});
#include "autogen_airwin/Deckwrecka.h"
int Deckwrecka_unused = AW2RModule::registerAirwindow({"Deckwrecka", "Unclassified", airwin2rack::Deckwrecka::kNumParameters, []() { return std::make_unique<airwin2rack::Deckwrecka::Deckwrecka>(0); }});
#include "autogen_airwin/Density.h"
int Density_unused = AW2RModule::registerAirwindow({"Density", "Unclassified", airwin2rack::Density::kNumParameters, []() { return std::make_unique<airwin2rack::Density::Density>(0); }});
#include "autogen_airwin/Density2.h"
int Density2_unused = AW2RModule::registerAirwindow({"Density2", "Unclassified", airwin2rack::Density2::kNumParameters, []() { return std::make_unique<airwin2rack::Density2::Density2>(0); }});
#include "autogen_airwin/Desk.h"
int Desk_unused = AW2RModule::registerAirwindow({"Desk", "Unclassified", airwin2rack::Desk::kNumParameters, []() { return std::make_unique<airwin2rack::Desk::Desk>(0); }});
#include "autogen_airwin/Desk4.h"
int Desk4_unused = AW2RModule::registerAirwindow({"Desk4", "Unclassified", airwin2rack::Desk4::kNumParameters, []() { return std::make_unique<airwin2rack::Desk4::Desk4>(0); }});
#include "autogen_airwin/DigitalBlack.h"
int DigitalBlack_unused = AW2RModule::registerAirwindow({"DigitalBlack", "Unclassified", airwin2rack::DigitalBlack::kNumParameters, []() { return std::make_unique<airwin2rack::DigitalBlack::DigitalBlack>(0); }});
#include "autogen_airwin/Dirt.h"
int Dirt_unused = AW2RModule::registerAirwindow({"Dirt", "Unclassified", airwin2rack::Dirt::kNumParameters, []() { return std::make_unique<airwin2rack::Dirt::Dirt>(0); }});
#include "autogen_airwin/Distance.h"
int Distance_unused = AW2RModule::registerAirwindow({"Distance", "Unclassified", airwin2rack::Distance::kNumParameters, []() { return std::make_unique<airwin2rack::Distance::Distance>(0); }});
#include "autogen_airwin/Distance2.h"
int Distance2_unused = AW2RModule::registerAirwindow({"Distance2", "Unclassified", airwin2rack::Distance2::kNumParameters, []() { return std::make_unique<airwin2rack::Distance2::Distance2>(0); }});
#include "autogen_airwin/Distortion.h"
int Distortion_unused = AW2RModule::registerAirwindow({"Distortion", "Unclassified", airwin2rack::Distortion::kNumParameters, []() { return std::make_unique<airwin2rack::Distortion::Distortion>(0); }});
#include "autogen_airwin/DitherFloat.h"
int DitherFloat_unused = AW2RModule::registerAirwindow({"DitherFloat", "Unclassified", airwin2rack::DitherFloat::kNumParameters, []() { return std::make_unique<airwin2rack::DitherFloat::DitherFloat>(0); }});
#include "autogen_airwin/DitherMeDiskers.h"
int DitherMeDiskers_unused = AW2RModule::registerAirwindow({"DitherMeDiskers", "Unclassified", airwin2rack::DitherMeDiskers::kNumParameters, []() { return std::make_unique<airwin2rack::DitherMeDiskers::DitherMeDiskers>(0); }});
#include "autogen_airwin/DitherMeTimbers.h"
int DitherMeTimbers_unused = AW2RModule::registerAirwindow({"DitherMeTimbers", "Unclassified", airwin2rack::DitherMeTimbers::kNumParameters, []() { return std::make_unique<airwin2rack::DitherMeTimbers::DitherMeTimbers>(0); }});
#include "autogen_airwin/Ditherbox.h"
int Ditherbox_unused = AW2RModule::registerAirwindow({"Ditherbox", "Unclassified", airwin2rack::Ditherbox::kNumParameters, []() { return std::make_unique<airwin2rack::Ditherbox::Ditherbox>(0); }});
#include "autogen_airwin/DoublePaul.h"
int DoublePaul_unused = AW2RModule::registerAirwindow({"DoublePaul", "Unclassified", airwin2rack::DoublePaul::kNumParameters, []() { return std::make_unique<airwin2rack::DoublePaul::DoublePaul>(0); }});
#include "autogen_airwin/Doublelay.h"
int Doublelay_unused = AW2RModule::registerAirwindow({"Doublelay", "Unclassified", airwin2rack::Doublelay::kNumParameters, []() { return std::make_unique<airwin2rack::Doublelay::Doublelay>(0); }});
#include "autogen_airwin/Drive.h"
int Drive_unused = AW2RModule::registerAirwindow({"Drive", "Unclassified", airwin2rack::Drive::kNumParameters, []() { return std::make_unique<airwin2rack::Drive::Drive>(0); }});
#include "autogen_airwin/DrumSlam.h"
int DrumSlam_unused = AW2RModule::registerAirwindow({"DrumSlam", "Unclassified", airwin2rack::DrumSlam::kNumParameters, []() { return std::make_unique<airwin2rack::DrumSlam::DrumSlam>(0); }});
#include "autogen_airwin/DubCenter.h"
int DubCenter_unused = AW2RModule::registerAirwindow({"DubCenter", "Unclassified", airwin2rack::DubCenter::kNumParameters, []() { return std::make_unique<airwin2rack::DubCenter::DubCenter>(0); }});
#include "autogen_airwin/DubSub.h"
int DubSub_unused = AW2RModule::registerAirwindow({"DubSub", "Unclassified", airwin2rack::DubSub::kNumParameters, []() { return std::make_unique<airwin2rack::DubSub::DubSub>(0); }});
#include "autogen_airwin/Dubly.h"
int Dubly_unused = AW2RModule::registerAirwindow({"Dubly", "Unclassified", airwin2rack::Dubly::kNumParameters, []() { return std::make_unique<airwin2rack::Dubly::Dubly>(0); }});
#include "autogen_airwin/DustBunny.h"
int DustBunny_unused = AW2RModule::registerAirwindow({"DustBunny", "Unclassified", airwin2rack::DustBunny::kNumParameters, []() { return std::make_unique<airwin2rack::DustBunny::DustBunny>(0); }});
#include "autogen_airwin/Dynamics.h"
int Dynamics_unused = AW2RModule::registerAirwindow({"Dynamics", "Unclassified", airwin2rack::Dynamics::kNumParameters, []() { return std::make_unique<airwin2rack::Dynamics::Dynamics>(0); }});
#include "autogen_airwin/Dyno.h"
int Dyno_unused = AW2RModule::registerAirwindow({"Dyno", "Unclassified", airwin2rack::Dyno::kNumParameters, []() { return std::make_unique<airwin2rack::Dyno::Dyno>(0); }});
#include "autogen_airwin/EdIsDim.h"
int EdIsDim_unused = AW2RModule::registerAirwindow({"EdIsDim", "Unclassified", airwin2rack::EdIsDim::kNumParameters, []() { return std::make_unique<airwin2rack::EdIsDim::EdIsDim>(0); }});
#include "autogen_airwin/Edge.h"
int Edge_unused = AW2RModule::registerAirwindow({"Edge", "Unclassified", airwin2rack::Edge::kNumParameters, []() { return std::make_unique<airwin2rack::Edge::Edge>(0); }});
#include "autogen_airwin/Elation.h"
int Elation_unused = AW2RModule::registerAirwindow({"Elation", "Unclassified", airwin2rack::Elation::kNumParameters, []() { return std::make_unique<airwin2rack::Elation::Elation>(0); }});
#include "autogen_airwin/ElectroHat.h"
int ElectroHat_unused = AW2RModule::registerAirwindow({"ElectroHat", "Unclassified", airwin2rack::ElectroHat::kNumParameters, []() { return std::make_unique<airwin2rack::ElectroHat::ElectroHat>(0); }});
#include "autogen_airwin/Energy.h"
int Energy_unused = AW2RModule::registerAirwindow({"Energy", "Unclassified", airwin2rack::Energy::kNumParameters, []() { return std::make_unique<airwin2rack::Energy::Energy>(0); }});
#include "autogen_airwin/Energy2.h"
int Energy2_unused = AW2RModule::registerAirwindow({"Energy2", "Unclassified", airwin2rack::Energy2::kNumParameters, []() { return std::make_unique<airwin2rack::Energy2::Energy2>(0); }});
#include "autogen_airwin/Ensemble.h"
int Ensemble_unused = AW2RModule::registerAirwindow({"Ensemble", "Unclassified", airwin2rack::Ensemble::kNumParameters, []() { return std::make_unique<airwin2rack::Ensemble::Ensemble>(0); }});
#include "autogen_airwin/EveryTrim.h"
int EveryTrim_unused = AW2RModule::registerAirwindow({"EveryTrim", "Unclassified", airwin2rack::EveryTrim::kNumParameters, []() { return std::make_unique<airwin2rack::EveryTrim::EveryTrim>(0); }});
#include "autogen_airwin/Exciter.h"
int Exciter_unused = AW2RModule::registerAirwindow({"Exciter", "Unclassified", airwin2rack::Exciter::kNumParameters, []() { return std::make_unique<airwin2rack::Exciter::Exciter>(0); }});
#include "autogen_airwin/Facet.h"
int Facet_unused = AW2RModule::registerAirwindow({"Facet", "Unclassified", airwin2rack::Facet::kNumParameters, []() { return std::make_unique<airwin2rack::Facet::Facet>(0); }});
#include "autogen_airwin/FathomFive.h"
int FathomFive_unused = AW2RModule::registerAirwindow({"FathomFive", "Unclassified", airwin2rack::FathomFive::kNumParameters, []() { return std::make_unique<airwin2rack::FathomFive::FathomFive>(0); }});
#include "autogen_airwin/FireAmp.h"
int FireAmp_unused = AW2RModule::registerAirwindow({"FireAmp", "Unclassified", airwin2rack::FireAmp::kNumParameters, []() { return std::make_unique<airwin2rack::FireAmp::FireAmp>(0); }});
#include "autogen_airwin/Flipity.h"
int Flipity_unused = AW2RModule::registerAirwindow({"Flipity", "Unclassified", airwin2rack::Flipity::kNumParameters, []() { return std::make_unique<airwin2rack::Flipity::Flipity>(0); }});
#include "autogen_airwin/Floor.h"
int Floor_unused = AW2RModule::registerAirwindow({"Floor", "Unclassified", airwin2rack::Floor::kNumParameters, []() { return std::make_unique<airwin2rack::Floor::Floor>(0); }});
#include "autogen_airwin/Flutter.h"
int Flutter_unused = AW2RModule::registerAirwindow({"Flutter", "Unclassified", airwin2rack::Flutter::kNumParameters, []() { return std::make_unique<airwin2rack::Flutter::Flutter>(0); }});
#include "autogen_airwin/Focus.h"
int Focus_unused = AW2RModule::registerAirwindow({"Focus", "Unclassified", airwin2rack::Focus::kNumParameters, []() { return std::make_unique<airwin2rack::Focus::Focus>(0); }});
#include "autogen_airwin/Fracture.h"
int Fracture_unused = AW2RModule::registerAirwindow({"Fracture", "Unclassified", airwin2rack::Fracture::kNumParameters, []() { return std::make_unique<airwin2rack::Fracture::Fracture>(0); }});
#include "autogen_airwin/Gatelope.h"
int Gatelope_unused = AW2RModule::registerAirwindow({"Gatelope", "Unclassified", airwin2rack::Gatelope::kNumParameters, []() { return std::make_unique<airwin2rack::Gatelope::Gatelope>(0); }});
#include "autogen_airwin/GlitchShifter.h"
int GlitchShifter_unused = AW2RModule::registerAirwindow({"GlitchShifter", "Unclassified", airwin2rack::GlitchShifter::kNumParameters, []() { return std::make_unique<airwin2rack::GlitchShifter::GlitchShifter>(0); }});
#include "autogen_airwin/Golem.h"
int Golem_unused = AW2RModule::registerAirwindow({"Golem", "Unclassified", airwin2rack::Golem::kNumParameters, []() { return std::make_unique<airwin2rack::Golem::Golem>(0); }});
#include "autogen_airwin/GrindAmp.h"
int GrindAmp_unused = AW2RModule::registerAirwindow({"GrindAmp", "Unclassified", airwin2rack::GrindAmp::kNumParameters, []() { return std::make_unique<airwin2rack::GrindAmp::GrindAmp>(0); }});
#include "autogen_airwin/Gringer.h"
int Gringer_unused = AW2RModule::registerAirwindow({"Gringer", "Unclassified", airwin2rack::Gringer::kNumParameters, []() { return std::make_unique<airwin2rack::Gringer::Gringer>(0); }});
#include "autogen_airwin/GrooveWear.h"
int GrooveWear_unused = AW2RModule::registerAirwindow({"GrooveWear", "Unclassified", airwin2rack::GrooveWear::kNumParameters, []() { return std::make_unique<airwin2rack::GrooveWear::GrooveWear>(0); }});
#include "autogen_airwin/GuitarConditioner.h"
int GuitarConditioner_unused = AW2RModule::registerAirwindow({"GuitarConditioner", "Unclassified", airwin2rack::GuitarConditioner::kNumParameters, []() { return std::make_unique<airwin2rack::GuitarConditioner::GuitarConditioner>(0); }});
#include "autogen_airwin/HardVacuum.h"
int HardVacuum_unused = AW2RModule::registerAirwindow({"HardVacuum", "Unclassified", airwin2rack::HardVacuum::kNumParameters, []() { return std::make_unique<airwin2rack::HardVacuum::HardVacuum>(0); }});
#include "autogen_airwin/HermeTrim.h"
int HermeTrim_unused = AW2RModule::registerAirwindow({"HermeTrim", "Unclassified", airwin2rack::HermeTrim::kNumParameters, []() { return std::make_unique<airwin2rack::HermeTrim::HermeTrim>(0); }});
#include "autogen_airwin/Hermepass.h"
int Hermepass_unused = AW2RModule::registerAirwindow({"Hermepass", "Unclassified", airwin2rack::Hermepass::kNumParameters, []() { return std::make_unique<airwin2rack::Hermepass::Hermepass>(0); }});
#include "autogen_airwin/HighGlossDither.h"
int HighGlossDither_unused = AW2RModule::registerAirwindow({"HighGlossDither", "Unclassified", airwin2rack::HighGlossDither::kNumParameters, []() { return std::make_unique<airwin2rack::HighGlossDither::HighGlossDither>(0); }});
#include "autogen_airwin/HighImpact.h"
int HighImpact_unused = AW2RModule::registerAirwindow({"HighImpact", "Unclassified", airwin2rack::HighImpact::kNumParameters, []() { return std::make_unique<airwin2rack::HighImpact::HighImpact>(0); }});
#include "autogen_airwin/Highpass.h"
int Highpass_unused = AW2RModule::registerAirwindow({"Highpass", "Unclassified", airwin2rack::Highpass::kNumParameters, []() { return std::make_unique<airwin2rack::Highpass::Highpass>(0); }});
#include "autogen_airwin/Highpass2.h"
int Highpass2_unused = AW2RModule::registerAirwindow({"Highpass2", "Unclassified", airwin2rack::Highpass2::kNumParameters, []() { return std::make_unique<airwin2rack::Highpass2::Highpass2>(0); }});
#include "autogen_airwin/Holt.h"
int Holt_unused = AW2RModule::registerAirwindow({"Holt", "Unclassified", airwin2rack::Holt::kNumParameters, []() { return std::make_unique<airwin2rack::Holt::Holt>(0); }});
#include "autogen_airwin/Holt2.h"
int Holt2_unused = AW2RModule::registerAirwindow({"Holt2", "Unclassified", airwin2rack::Holt2::kNumParameters, []() { return std::make_unique<airwin2rack::Holt2::Holt2>(0); }});
#include "autogen_airwin/Hombre.h"
int Hombre_unused = AW2RModule::registerAirwindow({"Hombre", "Unclassified", airwin2rack::Hombre::kNumParameters, []() { return std::make_unique<airwin2rack::Hombre::Hombre>(0); }});
#include "autogen_airwin/Hull.h"
int Hull_unused = AW2RModule::registerAirwindow({"Hull", "Unclassified", airwin2rack::Hull::kNumParameters, []() { return std::make_unique<airwin2rack::Hull::Hull>(0); }});
#include "autogen_airwin/Hype.h"
int Hype_unused = AW2RModule::registerAirwindow({"Hype", "Unclassified", airwin2rack::Hype::kNumParameters, []() { return std::make_unique<airwin2rack::Hype::Hype>(0); }});
#include "autogen_airwin/HypersonX.h"
int HypersonX_unused = AW2RModule::registerAirwindow({"HypersonX", "Unclassified", airwin2rack::HypersonX::kNumParameters, []() { return std::make_unique<airwin2rack::HypersonX::HypersonX>(0); }});
#include "autogen_airwin/Hypersonic.h"
int Hypersonic_unused = AW2RModule::registerAirwindow({"Hypersonic", "Unclassified", airwin2rack::Hypersonic::kNumParameters, []() { return std::make_unique<airwin2rack::Hypersonic::Hypersonic>(0); }});
#include "autogen_airwin/Infinity.h"
int Infinity_unused = AW2RModule::registerAirwindow({"Infinity", "Unclassified", airwin2rack::Infinity::kNumParameters, []() { return std::make_unique<airwin2rack::Infinity::Infinity>(0); }});
#include "autogen_airwin/Infinity2.h"
int Infinity2_unused = AW2RModule::registerAirwindow({"Infinity2", "Unclassified", airwin2rack::Infinity2::kNumParameters, []() { return std::make_unique<airwin2rack::Infinity2::Infinity2>(0); }});
#include "autogen_airwin/Infrasonic.h"
int Infrasonic_unused = AW2RModule::registerAirwindow({"Infrasonic", "Unclassified", airwin2rack::Infrasonic::kNumParameters, []() { return std::make_unique<airwin2rack::Infrasonic::Infrasonic>(0); }});
#include "autogen_airwin/Interstage.h"
int Interstage_unused = AW2RModule::registerAirwindow({"Interstage", "Unclassified", airwin2rack::Interstage::kNumParameters, []() { return std::make_unique<airwin2rack::Interstage::Interstage>(0); }});
#include "autogen_airwin/Isolator.h"
int Isolator_unused = AW2RModule::registerAirwindow({"Isolator", "Unclassified", airwin2rack::Isolator::kNumParameters, []() { return std::make_unique<airwin2rack::Isolator::Isolator>(0); }});
#include "autogen_airwin/Isolator2.h"
int Isolator2_unused = AW2RModule::registerAirwindow({"Isolator2", "Unclassified", airwin2rack::Isolator2::kNumParameters, []() { return std::make_unique<airwin2rack::Isolator2::Isolator2>(0); }});
#include "autogen_airwin/LRFlipTimer.h"
int LRFlipTimer_unused = AW2RModule::registerAirwindow({"LRFlipTimer", "Unclassified", airwin2rack::LRFlipTimer::kNumParameters, []() { return std::make_unique<airwin2rack::LRFlipTimer::LRFlipTimer>(0); }});
#include "autogen_airwin/LeadAmp.h"
int LeadAmp_unused = AW2RModule::registerAirwindow({"LeadAmp", "Unclassified", airwin2rack::LeadAmp::kNumParameters, []() { return std::make_unique<airwin2rack::LeadAmp::LeadAmp>(0); }});
#include "autogen_airwin/LeftoMono.h"
int LeftoMono_unused = AW2RModule::registerAirwindow({"LeftoMono", "Unclassified", airwin2rack::LeftoMono::kNumParameters, []() { return std::make_unique<airwin2rack::LeftoMono::LeftoMono>(0); }});
#include "autogen_airwin/LilAmp.h"
int LilAmp_unused = AW2RModule::registerAirwindow({"LilAmp", "Unclassified", airwin2rack::LilAmp::kNumParameters, []() { return std::make_unique<airwin2rack::LilAmp::LilAmp>(0); }});
#include "autogen_airwin/Logical4.h"
int Logical4_unused = AW2RModule::registerAirwindow({"Logical4", "Unclassified", airwin2rack::Logical4::kNumParameters, []() { return std::make_unique<airwin2rack::Logical4::Logical4>(0); }});
#include "autogen_airwin/Loud.h"
int Loud_unused = AW2RModule::registerAirwindow({"Loud", "Unclassified", airwin2rack::Loud::kNumParameters, []() { return std::make_unique<airwin2rack::Loud::Loud>(0); }});
#include "autogen_airwin/Lowpass.h"
int Lowpass_unused = AW2RModule::registerAirwindow({"Lowpass", "Unclassified", airwin2rack::Lowpass::kNumParameters, []() { return std::make_unique<airwin2rack::Lowpass::Lowpass>(0); }});
#include "autogen_airwin/Lowpass2.h"
int Lowpass2_unused = AW2RModule::registerAirwindow({"Lowpass2", "Unclassified", airwin2rack::Lowpass2::kNumParameters, []() { return std::make_unique<airwin2rack::Lowpass2::Lowpass2>(0); }});
#include "autogen_airwin/Luxor.h"
int Luxor_unused = AW2RModule::registerAirwindow({"Luxor", "Unclassified", airwin2rack::Luxor::kNumParameters, []() { return std::make_unique<airwin2rack::Luxor::Luxor>(0); }});
#include "autogen_airwin/MV.h"
int MV_unused = AW2RModule::registerAirwindow({"MV", "Unclassified", airwin2rack::MV::kNumParameters, []() { return std::make_unique<airwin2rack::MV::MV>(0); }});
#include "autogen_airwin/MackEQ.h"
int MackEQ_unused = AW2RModule::registerAirwindow({"MackEQ", "Unclassified", airwin2rack::MackEQ::kNumParameters, []() { return std::make_unique<airwin2rack::MackEQ::MackEQ>(0); }});
#include "autogen_airwin/Mackity.h"
int Mackity_unused = AW2RModule::registerAirwindow({"Mackity", "Unclassified", airwin2rack::Mackity::kNumParameters, []() { return std::make_unique<airwin2rack::Mackity::Mackity>(0); }});
#include "autogen_airwin/Melt.h"
int Melt_unused = AW2RModule::registerAirwindow({"Melt", "Unclassified", airwin2rack::Melt::kNumParameters, []() { return std::make_unique<airwin2rack::Melt::Melt>(0); }});
#include "autogen_airwin/MidAmp.h"
int MidAmp_unused = AW2RModule::registerAirwindow({"MidAmp", "Unclassified", airwin2rack::MidAmp::kNumParameters, []() { return std::make_unique<airwin2rack::MidAmp::MidAmp>(0); }});
#include "autogen_airwin/MidSide.h"
int MidSide_unused = AW2RModule::registerAirwindow({"MidSide", "Unclassified", airwin2rack::MidSide::kNumParameters, []() { return std::make_unique<airwin2rack::MidSide::MidSide>(0); }});
#include "autogen_airwin/MoNoam.h"
int MoNoam_unused = AW2RModule::registerAirwindow({"MoNoam", "Unclassified", airwin2rack::MoNoam::kNumParameters, []() { return std::make_unique<airwin2rack::MoNoam::MoNoam>(0); }});
#include "autogen_airwin/Mojo.h"
int Mojo_unused = AW2RModule::registerAirwindow({"Mojo", "Unclassified", airwin2rack::Mojo::kNumParameters, []() { return std::make_unique<airwin2rack::Mojo::Mojo>(0); }});
#include "autogen_airwin/Monitoring.h"
int Monitoring_unused = AW2RModule::registerAirwindow({"Monitoring", "Unclassified", airwin2rack::Monitoring::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring::Monitoring>(0); }});
#include "autogen_airwin/Monitoring2.h"
int Monitoring2_unused = AW2RModule::registerAirwindow({"Monitoring2", "Unclassified", airwin2rack::Monitoring2::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring2::Monitoring2>(0); }});
#include "autogen_airwin/Monitoring3.h"
int Monitoring3_unused = AW2RModule::registerAirwindow({"Monitoring3", "Unclassified", airwin2rack::Monitoring3::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring3::Monitoring3>(0); }});
#include "autogen_airwin/MultiBandDistortion.h"
int MultiBandDistortion_unused = AW2RModule::registerAirwindow({"MultiBandDistortion", "Unclassified", airwin2rack::MultiBandDistortion::kNumParameters, []() { return std::make_unique<airwin2rack::MultiBandDistortion::MultiBandDistortion>(0); }});
#include "autogen_airwin/NCSeventeen.h"
int NCSeventeen_unused = AW2RModule::registerAirwindow({"NCSeventeen", "Unclassified", airwin2rack::NCSeventeen::kNumParameters, []() { return std::make_unique<airwin2rack::NCSeventeen::NCSeventeen>(0); }});
#include "autogen_airwin/NaturalizeDither.h"
int NaturalizeDither_unused = AW2RModule::registerAirwindow({"NaturalizeDither", "Unclassified", airwin2rack::NaturalizeDither::kNumParameters, []() { return std::make_unique<airwin2rack::NaturalizeDither::NaturalizeDither>(0); }});
#include "autogen_airwin/Neverland.h"
int Neverland_unused = AW2RModule::registerAirwindow({"Neverland", "Unclassified", airwin2rack::Neverland::kNumParameters, []() { return std::make_unique<airwin2rack::Neverland::Neverland>(0); }});
#include "autogen_airwin/Nikola.h"
int Nikola_unused = AW2RModule::registerAirwindow({"Nikola", "Unclassified", airwin2rack::Nikola::kNumParameters, []() { return std::make_unique<airwin2rack::Nikola::Nikola>(0); }});
#include "autogen_airwin/NodeDither.h"
int NodeDither_unused = AW2RModule::registerAirwindow({"NodeDither", "Unclassified", airwin2rack::NodeDither::kNumParameters, []() { return std::make_unique<airwin2rack::NodeDither::NodeDither>(0); }});
#include "autogen_airwin/Noise.h"
int Noise_unused = AW2RModule::registerAirwindow({"Noise", "Unclassified", airwin2rack::Noise::kNumParameters, []() { return std::make_unique<airwin2rack::Noise::Noise>(0); }});
#include "autogen_airwin/NonlinearSpace.h"
int NonlinearSpace_unused = AW2RModule::registerAirwindow({"NonlinearSpace", "Unclassified", airwin2rack::NonlinearSpace::kNumParameters, []() { return std::make_unique<airwin2rack::NonlinearSpace::NonlinearSpace>(0); }});
#include "autogen_airwin/NotJustAnotherCD.h"
int NotJustAnotherCD_unused = AW2RModule::registerAirwindow({"NotJustAnotherCD", "Unclassified", airwin2rack::NotJustAnotherCD::kNumParameters, []() { return std::make_unique<airwin2rack::NotJustAnotherCD::NotJustAnotherCD>(0); }});
#include "autogen_airwin/NotJustAnotherDither.h"
int NotJustAnotherDither_unused = AW2RModule::registerAirwindow({"NotJustAnotherDither", "Unclassified", airwin2rack::NotJustAnotherDither::kNumParameters, []() { return std::make_unique<airwin2rack::NotJustAnotherDither::NotJustAnotherDither>(0); }});
#include "autogen_airwin/OneCornerClip.h"
int OneCornerClip_unused = AW2RModule::registerAirwindow({"OneCornerClip", "Unclassified", airwin2rack::OneCornerClip::kNumParameters, []() { return std::make_unique<airwin2rack::OneCornerClip::OneCornerClip>(0); }});
#include "autogen_airwin/PDBuss.h"
int PDBuss_unused = AW2RModule::registerAirwindow({"PDBuss", "Unclassified", airwin2rack::PDBuss::kNumParameters, []() { return std::make_unique<airwin2rack::PDBuss::PDBuss>(0); }});
#include "autogen_airwin/PDChannel.h"
int PDChannel_unused = AW2RModule::registerAirwindow({"PDChannel", "Unclassified", airwin2rack::PDChannel::kNumParameters, []() { return std::make_unique<airwin2rack::PDChannel::PDChannel>(0); }});
#include "autogen_airwin/Pafnuty.h"
int Pafnuty_unused = AW2RModule::registerAirwindow({"Pafnuty", "Unclassified", airwin2rack::Pafnuty::kNumParameters, []() { return std::make_unique<airwin2rack::Pafnuty::Pafnuty>(0); }});
#include "autogen_airwin/PaulDither.h"
int PaulDither_unused = AW2RModule::registerAirwindow({"PaulDither", "Unclassified", airwin2rack::PaulDither::kNumParameters, []() { return std::make_unique<airwin2rack::PaulDither::PaulDither>(0); }});
#include "autogen_airwin/PaulWide.h"
int PaulWide_unused = AW2RModule::registerAirwindow({"PaulWide", "Unclassified", airwin2rack::PaulWide::kNumParameters, []() { return std::make_unique<airwin2rack::PaulWide::PaulWide>(0); }});
#include "autogen_airwin/PeaksOnly.h"
int PeaksOnly_unused = AW2RModule::registerAirwindow({"PeaksOnly", "Unclassified", airwin2rack::PeaksOnly::kNumParameters, []() { return std::make_unique<airwin2rack::PeaksOnly::PeaksOnly>(0); }});
#include "autogen_airwin/PhaseNudge.h"
int PhaseNudge_unused = AW2RModule::registerAirwindow({"PhaseNudge", "Unclassified", airwin2rack::PhaseNudge::kNumParameters, []() { return std::make_unique<airwin2rack::PhaseNudge::PhaseNudge>(0); }});
#include "autogen_airwin/PitchDelay.h"
int PitchDelay_unused = AW2RModule::registerAirwindow({"PitchDelay", "Unclassified", airwin2rack::PitchDelay::kNumParameters, []() { return std::make_unique<airwin2rack::PitchDelay::PitchDelay>(0); }});
#include "autogen_airwin/PitchNasty.h"
int PitchNasty_unused = AW2RModule::registerAirwindow({"PitchNasty", "Unclassified", airwin2rack::PitchNasty::kNumParameters, []() { return std::make_unique<airwin2rack::PitchNasty::PitchNasty>(0); }});
#include "autogen_airwin/PocketVerbs.h"
int PocketVerbs_unused = AW2RModule::registerAirwindow({"PocketVerbs", "Unclassified", airwin2rack::PocketVerbs::kNumParameters, []() { return std::make_unique<airwin2rack::PocketVerbs::PocketVerbs>(0); }});
#include "autogen_airwin/Pockey.h"
int Pockey_unused = AW2RModule::registerAirwindow({"Pockey", "Unclassified", airwin2rack::Pockey::kNumParameters, []() { return std::make_unique<airwin2rack::Pockey::Pockey>(0); }});
#include "autogen_airwin/Pockey2.h"
int Pockey2_unused = AW2RModule::registerAirwindow({"Pockey2", "Unclassified", airwin2rack::Pockey2::kNumParameters, []() { return std::make_unique<airwin2rack::Pockey2::Pockey2>(0); }});
#include "autogen_airwin/Podcast.h"
int Podcast_unused = AW2RModule::registerAirwindow({"Podcast", "Unclassified", airwin2rack::Podcast::kNumParameters, []() { return std::make_unique<airwin2rack::Podcast::Podcast>(0); }});
#include "autogen_airwin/PodcastDeluxe.h"
int PodcastDeluxe_unused = AW2RModule::registerAirwindow({"PodcastDeluxe", "Unclassified", airwin2rack::PodcastDeluxe::kNumParameters, []() { return std::make_unique<airwin2rack::PodcastDeluxe::PodcastDeluxe>(0); }});
#include "autogen_airwin/Point.h"
int Point_unused = AW2RModule::registerAirwindow({"Point", "Unclassified", airwin2rack::Point::kNumParameters, []() { return std::make_unique<airwin2rack::Point::Point>(0); }});
#include "autogen_airwin/Pop.h"
int Pop_unused = AW2RModule::registerAirwindow({"Pop", "Unclassified", airwin2rack::Pop::kNumParameters, []() { return std::make_unique<airwin2rack::Pop::Pop>(0); }});
#include "autogen_airwin/Pop2.h"
int Pop2_unused = AW2RModule::registerAirwindow({"Pop2", "Unclassified", airwin2rack::Pop2::kNumParameters, []() { return std::make_unique<airwin2rack::Pop2::Pop2>(0); }});
#include "autogen_airwin/PowerSag.h"
int PowerSag_unused = AW2RModule::registerAirwindow({"PowerSag", "Unclassified", airwin2rack::PowerSag::kNumParameters, []() { return std::make_unique<airwin2rack::PowerSag::PowerSag>(0); }});
#include "autogen_airwin/PowerSag2.h"
int PowerSag2_unused = AW2RModule::registerAirwindow({"PowerSag2", "Unclassified", airwin2rack::PowerSag2::kNumParameters, []() { return std::make_unique<airwin2rack::PowerSag2::PowerSag2>(0); }});
#include "autogen_airwin/Precious.h"
int Precious_unused = AW2RModule::registerAirwindow({"Precious", "Unclassified", airwin2rack::Precious::kNumParameters, []() { return std::make_unique<airwin2rack::Precious::Precious>(0); }});
#include "autogen_airwin/Preponderant.h"
int Preponderant_unused = AW2RModule::registerAirwindow({"Preponderant", "Unclassified", airwin2rack::Preponderant::kNumParameters, []() { return std::make_unique<airwin2rack::Preponderant::Preponderant>(0); }});
#include "autogen_airwin/Pressure4.h"
int Pressure4_unused = AW2RModule::registerAirwindow({"Pressure4", "Unclassified", airwin2rack::Pressure4::kNumParameters, []() { return std::make_unique<airwin2rack::Pressure4::Pressure4>(0); }});
#include "autogen_airwin/Pressure5.h"
int Pressure5_unused = AW2RModule::registerAirwindow({"Pressure5", "Unclassified", airwin2rack::Pressure5::kNumParameters, []() { return std::make_unique<airwin2rack::Pressure5::Pressure5>(0); }});
#include "autogen_airwin/Pyewacket.h"
int Pyewacket_unused = AW2RModule::registerAirwindow({"Pyewacket", "Unclassified", airwin2rack::Pyewacket::kNumParameters, []() { return std::make_unique<airwin2rack::Pyewacket::Pyewacket>(0); }});
#include "autogen_airwin/RawGlitters.h"
int RawGlitters_unused = AW2RModule::registerAirwindow({"RawGlitters", "Unclassified", airwin2rack::RawGlitters::kNumParameters, []() { return std::make_unique<airwin2rack::RawGlitters::RawGlitters>(0); }});
#include "autogen_airwin/RawTimbers.h"
int RawTimbers_unused = AW2RModule::registerAirwindow({"RawTimbers", "Unclassified", airwin2rack::RawTimbers::kNumParameters, []() { return std::make_unique<airwin2rack::RawTimbers::RawTimbers>(0); }});
#include "autogen_airwin/Recurve.h"
int Recurve_unused = AW2RModule::registerAirwindow({"Recurve", "Unclassified", airwin2rack::Recurve::kNumParameters, []() { return std::make_unique<airwin2rack::Recurve::Recurve>(0); }});
#include "autogen_airwin/Remap.h"
int Remap_unused = AW2RModule::registerAirwindow({"Remap", "Unclassified", airwin2rack::Remap::kNumParameters, []() { return std::make_unique<airwin2rack::Remap::Remap>(0); }});
#include "autogen_airwin/Reverb.h"
int Reverb_unused = AW2RModule::registerAirwindow({"Reverb", "Unclassified", airwin2rack::Reverb::kNumParameters, []() { return std::make_unique<airwin2rack::Reverb::Reverb>(0); }});
#include "autogen_airwin/Righteous4.h"
int Righteous4_unused = AW2RModule::registerAirwindow({"Righteous4", "Unclassified", airwin2rack::Righteous4::kNumParameters, []() { return std::make_unique<airwin2rack::Righteous4::Righteous4>(0); }});
#include "autogen_airwin/RightoMono.h"
int RightoMono_unused = AW2RModule::registerAirwindow({"RightoMono", "Unclassified", airwin2rack::RightoMono::kNumParameters, []() { return std::make_unique<airwin2rack::RightoMono::RightoMono>(0); }});
#include "autogen_airwin/Shape.h"
int Shape_unused = AW2RModule::registerAirwindow({"Shape", "Unclassified", airwin2rack::Shape::kNumParameters, []() { return std::make_unique<airwin2rack::Shape::Shape>(0); }});
#include "autogen_airwin/SideDull.h"
int SideDull_unused = AW2RModule::registerAirwindow({"SideDull", "Unclassified", airwin2rack::SideDull::kNumParameters, []() { return std::make_unique<airwin2rack::SideDull::SideDull>(0); }});
#include "autogen_airwin/Sidepass.h"
int Sidepass_unused = AW2RModule::registerAirwindow({"Sidepass", "Unclassified", airwin2rack::Sidepass::kNumParameters, []() { return std::make_unique<airwin2rack::Sidepass::Sidepass>(0); }});
#include "autogen_airwin/Silhouette.h"
int Silhouette_unused = AW2RModule::registerAirwindow({"Silhouette", "Unclassified", airwin2rack::Silhouette::kNumParameters, []() { return std::make_unique<airwin2rack::Silhouette::Silhouette>(0); }});
#include "autogen_airwin/SingleEndedTriode.h"
int SingleEndedTriode_unused = AW2RModule::registerAirwindow({"SingleEndedTriode", "Unclassified", airwin2rack::SingleEndedTriode::kNumParameters, []() { return std::make_unique<airwin2rack::SingleEndedTriode::SingleEndedTriode>(0); }});
#include "autogen_airwin/Slew.h"
int Slew_unused = AW2RModule::registerAirwindow({"Slew", "Unclassified", airwin2rack::Slew::kNumParameters, []() { return std::make_unique<airwin2rack::Slew::Slew>(0); }});
#include "autogen_airwin/Slew2.h"
int Slew2_unused = AW2RModule::registerAirwindow({"Slew2", "Unclassified", airwin2rack::Slew2::kNumParameters, []() { return std::make_unique<airwin2rack::Slew2::Slew2>(0); }});
#include "autogen_airwin/Slew3.h"
int Slew3_unused = AW2RModule::registerAirwindow({"Slew3", "Unclassified", airwin2rack::Slew3::kNumParameters, []() { return std::make_unique<airwin2rack::Slew3::Slew3>(0); }});
#include "autogen_airwin/SlewOnly.h"
int SlewOnly_unused = AW2RModule::registerAirwindow({"SlewOnly", "Unclassified", airwin2rack::SlewOnly::kNumParameters, []() { return std::make_unique<airwin2rack::SlewOnly::SlewOnly>(0); }});
#include "autogen_airwin/Smooth.h"
int Smooth_unused = AW2RModule::registerAirwindow({"Smooth", "Unclassified", airwin2rack::Smooth::kNumParameters, []() { return std::make_unique<airwin2rack::Smooth::Smooth>(0); }});
#include "autogen_airwin/SoftGate.h"
int SoftGate_unused = AW2RModule::registerAirwindow({"SoftGate", "Unclassified", airwin2rack::SoftGate::kNumParameters, []() { return std::make_unique<airwin2rack::SoftGate::SoftGate>(0); }});
#include "autogen_airwin/SpatializeDither.h"
int SpatializeDither_unused = AW2RModule::registerAirwindow({"SpatializeDither", "Unclassified", airwin2rack::SpatializeDither::kNumParameters, []() { return std::make_unique<airwin2rack::SpatializeDither::SpatializeDither>(0); }});
#include "autogen_airwin/Spiral.h"
int Spiral_unused = AW2RModule::registerAirwindow({"Spiral", "Unclassified", airwin2rack::Spiral::kNumParameters, []() { return std::make_unique<airwin2rack::Spiral::Spiral>(0); }});
#include "autogen_airwin/Spiral2.h"
int Spiral2_unused = AW2RModule::registerAirwindow({"Spiral2", "Unclassified", airwin2rack::Spiral2::kNumParameters, []() { return std::make_unique<airwin2rack::Spiral2::Spiral2>(0); }});
#include "autogen_airwin/Srsly.h"
int Srsly_unused = AW2RModule::registerAirwindow({"Srsly", "Unclassified", airwin2rack::Srsly::kNumParameters, []() { return std::make_unique<airwin2rack::Srsly::Srsly>(0); }});
#include "autogen_airwin/Srsly2.h"
int Srsly2_unused = AW2RModule::registerAirwindow({"Srsly2", "Unclassified", airwin2rack::Srsly2::kNumParameters, []() { return std::make_unique<airwin2rack::Srsly2::Srsly2>(0); }});
#include "autogen_airwin/StarChild.h"
int StarChild_unused = AW2RModule::registerAirwindow({"StarChild", "Unclassified", airwin2rack::StarChild::kNumParameters, []() { return std::make_unique<airwin2rack::StarChild::StarChild>(0); }});
#include "autogen_airwin/StereoChorus.h"
int StereoChorus_unused = AW2RModule::registerAirwindow({"StereoChorus", "Unclassified", airwin2rack::StereoChorus::kNumParameters, []() { return std::make_unique<airwin2rack::StereoChorus::StereoChorus>(0); }});
#include "autogen_airwin/StereoDoubler.h"
int StereoDoubler_unused = AW2RModule::registerAirwindow({"StereoDoubler", "Unclassified", airwin2rack::StereoDoubler::kNumParameters, []() { return std::make_unique<airwin2rack::StereoDoubler::StereoDoubler>(0); }});
#include "autogen_airwin/StereoEnsemble.h"
int StereoEnsemble_unused = AW2RModule::registerAirwindow({"StereoEnsemble", "Unclassified", airwin2rack::StereoEnsemble::kNumParameters, []() { return std::make_unique<airwin2rack::StereoEnsemble::StereoEnsemble>(0); }});
#include "autogen_airwin/StereoFX.h"
int StereoFX_unused = AW2RModule::registerAirwindow({"StereoFX", "Unclassified", airwin2rack::StereoFX::kNumParameters, []() { return std::make_unique<airwin2rack::StereoFX::StereoFX>(0); }});
#include "autogen_airwin/StudioTan.h"
int StudioTan_unused = AW2RModule::registerAirwindow({"StudioTan", "Unclassified", airwin2rack::StudioTan::kNumParameters, []() { return std::make_unique<airwin2rack::StudioTan::StudioTan>(0); }});
#include "autogen_airwin/SubsOnly.h"
int SubsOnly_unused = AW2RModule::registerAirwindow({"SubsOnly", "Unclassified", airwin2rack::SubsOnly::kNumParameters, []() { return std::make_unique<airwin2rack::SubsOnly::SubsOnly>(0); }});
#include "autogen_airwin/Surge.h"
int Surge_unused = AW2RModule::registerAirwindow({"Surge", "Unclassified", airwin2rack::Surge::kNumParameters, []() { return std::make_unique<airwin2rack::Surge::Surge>(0); }});
#include "autogen_airwin/SurgeTide.h"
int SurgeTide_unused = AW2RModule::registerAirwindow({"SurgeTide", "Unclassified", airwin2rack::SurgeTide::kNumParameters, []() { return std::make_unique<airwin2rack::SurgeTide::SurgeTide>(0); }});
#include "autogen_airwin/Swell.h"
int Swell_unused = AW2RModule::registerAirwindow({"Swell", "Unclassified", airwin2rack::Swell::kNumParameters, []() { return std::make_unique<airwin2rack::Swell::Swell>(0); }});
#include "autogen_airwin/TPDFDither.h"
int TPDFDither_unused = AW2RModule::registerAirwindow({"TPDFDither", "Unclassified", airwin2rack::TPDFDither::kNumParameters, []() { return std::make_unique<airwin2rack::TPDFDither::TPDFDither>(0); }});
#include "autogen_airwin/TPDFWide.h"
int TPDFWide_unused = AW2RModule::registerAirwindow({"TPDFWide", "Unclassified", airwin2rack::TPDFWide::kNumParameters, []() { return std::make_unique<airwin2rack::TPDFWide::TPDFWide>(0); }});
#include "autogen_airwin/Texturize.h"
int Texturize_unused = AW2RModule::registerAirwindow({"Texturize", "Unclassified", airwin2rack::Texturize::kNumParameters, []() { return std::make_unique<airwin2rack::Texturize::Texturize>(0); }});
#include "autogen_airwin/TexturizeMS.h"
int TexturizeMS_unused = AW2RModule::registerAirwindow({"TexturizeMS", "Unclassified", airwin2rack::TexturizeMS::kNumParameters, []() { return std::make_unique<airwin2rack::TexturizeMS::TexturizeMS>(0); }});
#include "autogen_airwin/Thunder.h"
int Thunder_unused = AW2RModule::registerAirwindow({"Thunder", "Unclassified", airwin2rack::Thunder::kNumParameters, []() { return std::make_unique<airwin2rack::Thunder::Thunder>(0); }});
#include "autogen_airwin/ToneSlant.h"
int ToneSlant_unused = AW2RModule::registerAirwindow({"ToneSlant", "Unclassified", airwin2rack::ToneSlant::kNumParameters, []() { return std::make_unique<airwin2rack::ToneSlant::ToneSlant>(0); }});
#include "autogen_airwin/TransDesk.h"
int TransDesk_unused = AW2RModule::registerAirwindow({"TransDesk", "Unclassified", airwin2rack::TransDesk::kNumParameters, []() { return std::make_unique<airwin2rack::TransDesk::TransDesk>(0); }});
#include "autogen_airwin/TremoSquare.h"
int TremoSquare_unused = AW2RModule::registerAirwindow({"TremoSquare", "Unclassified", airwin2rack::TremoSquare::kNumParameters, []() { return std::make_unique<airwin2rack::TremoSquare::TremoSquare>(0); }});
#include "autogen_airwin/Tremolo.h"
int Tremolo_unused = AW2RModule::registerAirwindow({"Tremolo", "Unclassified", airwin2rack::Tremolo::kNumParameters, []() { return std::make_unique<airwin2rack::Tremolo::Tremolo>(0); }});
#include "autogen_airwin/TripleSpread.h"
int TripleSpread_unused = AW2RModule::registerAirwindow({"TripleSpread", "Unclassified", airwin2rack::TripleSpread::kNumParameters, []() { return std::make_unique<airwin2rack::TripleSpread::TripleSpread>(0); }});
#include "autogen_airwin/Tube.h"
int Tube_unused = AW2RModule::registerAirwindow({"Tube", "Unclassified", airwin2rack::Tube::kNumParameters, []() { return std::make_unique<airwin2rack::Tube::Tube>(0); }});
#include "autogen_airwin/Tube2.h"
int Tube2_unused = AW2RModule::registerAirwindow({"Tube2", "Unclassified", airwin2rack::Tube2::kNumParameters, []() { return std::make_unique<airwin2rack::Tube2::Tube2>(0); }});
#include "autogen_airwin/TubeDesk.h"
int TubeDesk_unused = AW2RModule::registerAirwindow({"TubeDesk", "Unclassified", airwin2rack::TubeDesk::kNumParameters, []() { return std::make_unique<airwin2rack::TubeDesk::TubeDesk>(0); }});
#include "autogen_airwin/UltrasonX.h"
int UltrasonX_unused = AW2RModule::registerAirwindow({"UltrasonX", "Unclassified", airwin2rack::UltrasonX::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonX::UltrasonX>(0); }});
#include "autogen_airwin/Ultrasonic.h"
int Ultrasonic_unused = AW2RModule::registerAirwindow({"Ultrasonic", "Unclassified", airwin2rack::Ultrasonic::kNumParameters, []() { return std::make_unique<airwin2rack::Ultrasonic::Ultrasonic>(0); }});
#include "autogen_airwin/UltrasonicLite.h"
int UltrasonicLite_unused = AW2RModule::registerAirwindow({"UltrasonicLite", "Unclassified", airwin2rack::UltrasonicLite::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonicLite::UltrasonicLite>(0); }});
#include "autogen_airwin/UltrasonicMed.h"
int UltrasonicMed_unused = AW2RModule::registerAirwindow({"UltrasonicMed", "Unclassified", airwin2rack::UltrasonicMed::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonicMed::UltrasonicMed>(0); }});
#include "autogen_airwin/UnBox.h"
int UnBox_unused = AW2RModule::registerAirwindow({"UnBox", "Unclassified", airwin2rack::UnBox::kNumParameters, []() { return std::make_unique<airwin2rack::UnBox::UnBox>(0); }});
#include "autogen_airwin/VariMu.h"
int VariMu_unused = AW2RModule::registerAirwindow({"VariMu", "Unclassified", airwin2rack::VariMu::kNumParameters, []() { return std::make_unique<airwin2rack::VariMu::VariMu>(0); }});
#include "autogen_airwin/Verbity.h"
int Verbity_unused = AW2RModule::registerAirwindow({"Verbity", "Unclassified", airwin2rack::Verbity::kNumParameters, []() { return std::make_unique<airwin2rack::Verbity::Verbity>(0); }});
#include "autogen_airwin/Verbity2.h"
int Verbity2_unused = AW2RModule::registerAirwindow({"Verbity2", "Unclassified", airwin2rack::Verbity2::kNumParameters, []() { return std::make_unique<airwin2rack::Verbity2::Verbity2>(0); }});
#include "autogen_airwin/Vibrato.h"
int Vibrato_unused = AW2RModule::registerAirwindow({"Vibrato", "Unclassified", airwin2rack::Vibrato::kNumParameters, []() { return std::make_unique<airwin2rack::Vibrato::Vibrato>(0); }});
#include "autogen_airwin/VinylDither.h"
int VinylDither_unused = AW2RModule::registerAirwindow({"VinylDither", "Unclassified", airwin2rack::VinylDither::kNumParameters, []() { return std::make_unique<airwin2rack::VinylDither::VinylDither>(0); }});
#include "autogen_airwin/VoiceOfTheStarship.h"
int VoiceOfTheStarship_unused = AW2RModule::registerAirwindow({"VoiceOfTheStarship", "Unclassified", airwin2rack::VoiceOfTheStarship::kNumParameters, []() { return std::make_unique<airwin2rack::VoiceOfTheStarship::VoiceOfTheStarship>(0); }});
#include "autogen_airwin/VoiceTrick.h"
int VoiceTrick_unused = AW2RModule::registerAirwindow({"VoiceTrick", "Unclassified", airwin2rack::VoiceTrick::kNumParameters, []() { return std::make_unique<airwin2rack::VoiceTrick::VoiceTrick>(0); }});
#include "autogen_airwin/Wider.h"
int Wider_unused = AW2RModule::registerAirwindow({"Wider", "Unclassified", airwin2rack::Wider::kNumParameters, []() { return std::make_unique<airwin2rack::Wider::Wider>(0); }});
#include "autogen_airwin/curve.h"
int curve_unused = AW2RModule::registerAirwindow({"curve", "Unclassified", airwin2rack::curve::kNumParameters, []() { return std::make_unique<airwin2rack::curve::curve>(0); }});
#include "autogen_airwin/uLawDecode.h"
int uLawDecode_unused = AW2RModule::registerAirwindow({"uLawDecode", "Unclassified", airwin2rack::uLawDecode::kNumParameters, []() { return std::make_unique<airwin2rack::uLawDecode::uLawDecode>(0); }});
#include "autogen_airwin/uLawEncode.h"
int uLawEncode_unused = AW2RModule::registerAirwindow({"uLawEncode", "Unclassified", airwin2rack::uLawEncode::kNumParameters, []() { return std::make_unique<airwin2rack::uLawEncode::uLawEncode>(0); }});
int unusedComplete = AW2RModule::completeRegistry();
