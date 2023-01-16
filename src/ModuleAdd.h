#include "autogen_airwin/Chamber.h"
int Chamber_unused = AW2RModule::registerAirwindow({"Chamber", "Reverbs", airwin2rack::Chamber::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber::Chamber>(0); }});
#include "autogen_airwin/Galactic.h"
int Galactic_unused = AW2RModule::registerAirwindow({"Galactic", "Reverbs", airwin2rack::Galactic::kNumParameters, []() { return std::make_unique<airwin2rack::Galactic::Galactic>(0); }});
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
#include "autogen_airwin/VoiceOfTheStarship.h"
int VoiceOfTheStarship_unused = AW2RModule::registerAirwindow({"VoiceOfTheStarship", "Noise", airwin2rack::VoiceOfTheStarship::kNumParameters, []() { return std::make_unique<airwin2rack::VoiceOfTheStarship::VoiceOfTheStarship>(0); }});
#include "autogen_airwin/GrooveWear.h"
int GrooveWear_unused = AW2RModule::registerAirwindow({"GrooveWear", "Tape and Vinyl", airwin2rack::GrooveWear::kNumParameters, []() { return std::make_unique<airwin2rack::GrooveWear::GrooveWear>(0); }});
#include "autogen_airwin/IronOxide5.h"
int IronOxide5_unused = AW2RModule::registerAirwindow({"IronOxide5", "Tape and Vinyl", airwin2rack::IronOxide5::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxide5::IronOxide5>(0); }});
#include "autogen_airwin/IronOxideClassic.h"
int IronOxideClassic_unused = AW2RModule::registerAirwindow({"IronOxideClassic", "Tape and Vinyl", airwin2rack::IronOxideClassic::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic::IronOxideClassic>(0); }});
#include "autogen_airwin/IronOxideClassic2.h"
int IronOxideClassic2_unused = AW2RModule::registerAirwindow({"IronOxideClassic2", "Tape and Vinyl", airwin2rack::IronOxideClassic2::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic2::IronOxideClassic2>(0); }});
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
#include "autogen_airwin/ADClip7.h"
int ADClip7_unused = AW2RModule::registerAirwindow({"ADClip7", "Distortion", airwin2rack::ADClip7::kNumParameters, []() { return std::make_unique<airwin2rack::ADClip7::ADClip7>(0); }});
#include "autogen_airwin/Cojones.h"
int Cojones_unused = AW2RModule::registerAirwindow({"Cojones", "Distortion", airwin2rack::Cojones::kNumParameters, []() { return std::make_unique<airwin2rack::Cojones::Cojones>(0); }});
#include "autogen_airwin/Distortion.h"
int Distortion_unused = AW2RModule::registerAirwindow({"Distortion", "Distortion", airwin2rack::Distortion::kNumParameters, []() { return std::make_unique<airwin2rack::Distortion::Distortion>(0); }});
#include "autogen_airwin/Hombre.h"
int Hombre_unused = AW2RModule::registerAirwindow({"Hombre", "Unclassified", airwin2rack::Hombre::kNumParameters, []() { return std::make_unique<airwin2rack::Hombre::Hombre>(0); }});
int unusedComplete = AW2RModule::completeRegistry();
