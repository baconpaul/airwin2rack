#include "autogen_airwin/ADClip7.h"
int ADClip7_unused = AirwinRegistry::registerAirwindow({"ADClip7", "Clipping", 75, "ADClip7 is the ultimate Airwindows loudness maximizer/ peak control.", airwin2rack::ADClip7::kNumParameters, []() { return std::make_unique<airwin2rack::ADClip7::ADClip7>(0); }});
#include "autogen_airwin/ADClip8.h"
int ADClip8_unused = AirwinRegistry::registerAirwindow({"ADClip8", "Unclassified", -1, "", airwin2rack::ADClip8::kNumParameters, []() { return std::make_unique<airwin2rack::ADClip8::ADClip8>(0); }});
#include "autogen_airwin/ADT.h"
int ADT_unused = AirwinRegistry::registerAirwindow({"ADT", "Ambience", 10, "ADT is a double short delay tap with saturation.", airwin2rack::ADT::kNumParameters, []() { return std::make_unique<airwin2rack::ADT::ADT>(0); }});
#include "autogen_airwin/AQuickVoiceClip.h"
int AQuickVoiceClip_unused = AirwinRegistry::registerAirwindow({"AQuickVoiceClip", "Clipping", 76, "AQuickVoiceClip softens headset mic recordings that have been super hard clipped on capture.", airwin2rack::AQuickVoiceClip::kNumParameters, []() { return std::make_unique<airwin2rack::AQuickVoiceClip::AQuickVoiceClip>(0); }});
#include "autogen_airwin/Acceleration.h"
int Acceleration_unused = AirwinRegistry::registerAirwindow({"Acceleration", "Brightness", 64, "Acceleration is an acceleration limiter that tames edge, leaves brightness.", airwin2rack::Acceleration::kNumParameters, []() { return std::make_unique<airwin2rack::Acceleration::Acceleration>(0); }});
#include "autogen_airwin/Acceleration2.h"
int Acceleration2_unused = AirwinRegistry::registerAirwindow({"Acceleration2", "Brightness", 54, "Acceleration2 is Acceleration, better! Updated for high sample rate and more extreme effect.", airwin2rack::Acceleration2::kNumParameters, []() { return std::make_unique<airwin2rack::Acceleration2::Acceleration2>(0); }});
#include "autogen_airwin/Air.h"
int Air_unused = AirwinRegistry::registerAirwindow({"Air", "Brightness", 62, "Air is a different bright EQ from any other. Requires 44.1K.", airwin2rack::Air::kNumParameters, []() { return std::make_unique<airwin2rack::Air::Air>(0); }});
#include "autogen_airwin/Air2.h"
int Air2_unused = AirwinRegistry::registerAirwindow({"Air2", "Brightness", 61, "Air2 is a different bright EQ with silk tone and high sample rate support.", airwin2rack::Air2::kNumParameters, []() { return std::make_unique<airwin2rack::Air2::Air2>(0); }});
#include "autogen_airwin/Air3.h"
int Air3_unused = AirwinRegistry::registerAirwindow({"Air3", "Unclassified", -1, "", airwin2rack::Air3::kNumParameters, []() { return std::make_unique<airwin2rack::Air3::Air3>(0); }});
#include "autogen_airwin/Apicolypse.h"
int Apicolypse_unused = AirwinRegistry::registerAirwindow({"Apicolypse", "Tone Color", 321, "Apicolypse is a re-release of my old API-style color adder, exacly as it was.", airwin2rack::Apicolypse::kNumParameters, []() { return std::make_unique<airwin2rack::Apicolypse::Apicolypse>(0); }});
#include "autogen_airwin/AtmosphereBuss.h"
int AtmosphereBuss_unused = AirwinRegistry::registerAirwindow({"AtmosphereBuss", "Consoles", 116, "AtmosphereBuss is Console5 processing with powerful new acoustic distance effects.", airwin2rack::AtmosphereBuss::kNumParameters, []() { return std::make_unique<airwin2rack::AtmosphereBuss::AtmosphereBuss>(0); }});
#include "autogen_airwin/AtmosphereChannel.h"
int AtmosphereChannel_unused = AirwinRegistry::registerAirwindow({"AtmosphereChannel", "Consoles", 117, "AtmosphereChannel is Console5 processing with powerful new acoustic distance effects.", airwin2rack::AtmosphereChannel::kNumParameters, []() { return std::make_unique<airwin2rack::AtmosphereChannel::AtmosphereChannel>(0); }});
#include "autogen_airwin/Aura.h"
int Aura_unused = AirwinRegistry::registerAirwindow({"Aura", "Effects", 191, "Aura is a new kind of resonant lowpass EQ.", airwin2rack::Aura::kNumParameters, []() { return std::make_unique<airwin2rack::Aura::Aura>(0); }});
#include "autogen_airwin/AutoPan.h"
int AutoPan_unused = AirwinRegistry::registerAirwindow({"AutoPan", "Stereo", 292, "AutoPan is for getting some organic stereo movement into your mix.", airwin2rack::AutoPan::kNumParameters, []() { return std::make_unique<airwin2rack::AutoPan::AutoPan>(0); }});
#include "autogen_airwin/AverMatrix.h"
int AverMatrix_unused = AirwinRegistry::registerAirwindow({"AverMatrix", "Filter", 216, "AverMatrix turns averaging into a full-featured EQ.", airwin2rack::AverMatrix::kNumParameters, []() { return std::make_unique<airwin2rack::AverMatrix::AverMatrix>(0); }});
#include "autogen_airwin/Average.h"
int Average_unused = AirwinRegistry::registerAirwindow({"Average", "Filter", 217, "Average is a distinctive sort of lowpass filter.", airwin2rack::Average::kNumParameters, []() { return std::make_unique<airwin2rack::Average::Average>(0); }});
#include "autogen_airwin/Balanced.h"
int Balanced_unused = AirwinRegistry::registerAirwindow({"Balanced", "Utility", 354, "Balanced lets you run an XLR input into a stereo input, sum it to mono, and use it.", airwin2rack::Balanced::kNumParameters, []() { return std::make_unique<airwin2rack::Balanced::Balanced>(0); }});
#include "autogen_airwin/BassAmp.h"
int BassAmp_unused = AirwinRegistry::registerAirwindow({"BassAmp", "Amp Sims", 34, "BassAmp is an old plugin with new tweaks, that gives some bass guitar tones.", airwin2rack::BassAmp::kNumParameters, []() { return std::make_unique<airwin2rack::BassAmp::BassAmp>(0); }});
#include "autogen_airwin/BassDrive.h"
int BassDrive_unused = AirwinRegistry::registerAirwindow({"BassDrive", "Amp Sims", 33, "BassDrive is an old secret weapon, like a kind of bass amp.", airwin2rack::BassDrive::kNumParameters, []() { return std::make_unique<airwin2rack::BassDrive::BassDrive>(0); }});
#include "autogen_airwin/BassKit.h"
int BassKit_unused = AirwinRegistry::registerAirwindow({"BassKit", "Bass", 37, "BassKit is centered bass reinforcement with subs fill. Clean and controllable.", airwin2rack::BassKit::kNumParameters, []() { return std::make_unique<airwin2rack::BassKit::BassKit>(0); }});
#include "autogen_airwin/Baxandall.h"
int Baxandall_unused = AirwinRegistry::registerAirwindow({"Baxandall", "Filter", 220, "Baxandall is a killer general-purpose EQ.", airwin2rack::Baxandall::kNumParameters, []() { return std::make_unique<airwin2rack::Baxandall::Baxandall>(0); }});
#include "autogen_airwin/Baxandall2.h"
int Baxandall2_unused = AirwinRegistry::registerAirwindow({"Baxandall2", "Filter", 205, "Baxandall2 is Baxandall, extended and made more powerful.", airwin2rack::Baxandall2::kNumParameters, []() { return std::make_unique<airwin2rack::Baxandall2::Baxandall2>(0); }});
#include "autogen_airwin/Beam.h"
int Beam_unused = AirwinRegistry::registerAirwindow({"Beam", "Dithers", 140, "Beam is a wordlength reducer that tries to heighten sonority.", airwin2rack::Beam::kNumParameters, []() { return std::make_unique<airwin2rack::Beam::Beam>(0); }});
#include "autogen_airwin/BigAmp.h"
int BigAmp_unused = AirwinRegistry::registerAirwindow({"BigAmp", "Amp Sims", 31, "BigAmp is a very flexible amplike thing with a taste for the bizarre.", airwin2rack::BigAmp::kNumParameters, []() { return std::make_unique<airwin2rack::BigAmp::BigAmp>(0); }});
#include "autogen_airwin/Biquad.h"
int Biquad_unused = AirwinRegistry::registerAirwindow({"Biquad", "Biquads", 44, "Biquad is the Airwindows implementation of a biquad filter.", airwin2rack::Biquad::kNumParameters, []() { return std::make_unique<airwin2rack::Biquad::Biquad>(0); }});
#include "autogen_airwin/Biquad2.h"
int Biquad2_unused = AirwinRegistry::registerAirwindow({"Biquad2", "Biquads", 48, "Biquad2 is the Airwindows biquad filter that's more sweepable and synthy.", airwin2rack::Biquad2::kNumParameters, []() { return std::make_unique<airwin2rack::Biquad2::Biquad2>(0); }});
#include "autogen_airwin/BiquadDouble.h"
int BiquadDouble_unused = AirwinRegistry::registerAirwindow({"BiquadDouble", "Biquads", 45, "BiquadDouble is a handy Airwindows cascaded biquad filter: steeper roll-off before resonance.", airwin2rack::BiquadDouble::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadDouble::BiquadDouble>(0); }});
#include "autogen_airwin/BiquadOneHalf.h"
int BiquadOneHalf_unused = AirwinRegistry::registerAirwindow({"BiquadOneHalf", "Biquads", 46, "BiquadOneHalf is an interleaved biquad filter like Biquad.", airwin2rack::BiquadOneHalf::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadOneHalf::BiquadOneHalf>(0); }});
#include "autogen_airwin/BiquadPlus.h"
int BiquadPlus_unused = AirwinRegistry::registerAirwindow({"BiquadPlus", "Biquads", 43, "BiquadPlus is Biquad plus zipper noise suppression! For twiddling the controls.", airwin2rack::BiquadPlus::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadPlus::BiquadPlus>(0); }});
#include "autogen_airwin/BiquadTriple.h"
int BiquadTriple_unused = AirwinRegistry::registerAirwindow({"BiquadTriple", "Biquads", 47, "BiquadTriple is a handy Airwindows cascaded biquad filter: steeper roll-off before resonance.", airwin2rack::BiquadTriple::kNumParameters, []() { return std::make_unique<airwin2rack::BiquadTriple::BiquadTriple>(0); }});
#include "autogen_airwin/BitGlitter.h"
int BitGlitter_unused = AirwinRegistry::registerAirwindow({"BitGlitter", "Lo-Fi", 239, "BitGlitter is an old-sampler style digital lo-fi plugin.", airwin2rack::BitGlitter::kNumParameters, []() { return std::make_unique<airwin2rack::BitGlitter::BitGlitter>(0); }});
#include "autogen_airwin/BitShiftGain.h"
int BitShiftGain_unused = AirwinRegistry::registerAirwindow({"BitShiftGain", "Utility", 338, "BitShiftGain is the ‘One Weird Trick’ perfect boost/pad, but in 6db increments ONLY.", airwin2rack::BitShiftGain::kNumParameters, []() { return std::make_unique<airwin2rack::BitShiftGain::BitShiftGain>(0); }});
#include "autogen_airwin/BitShiftPan.h"
int BitShiftPan_unused = AirwinRegistry::registerAirwindow({"BitShiftPan", "Utility", 337, "BitShiftPan gives you a gain and a pan control that are ONLY done using bit shifts.", airwin2rack::BitShiftPan::kNumParameters, []() { return std::make_unique<airwin2rack::BitShiftPan::BitShiftPan>(0); }});
#include "autogen_airwin/Bite.h"
int Bite_unused = AirwinRegistry::registerAirwindow({"Bite", "Lo-Fi", 244, "Bite is an unusual edge-maker.", airwin2rack::Bite::kNumParameters, []() { return std::make_unique<airwin2rack::Bite::Bite>(0); }});
#include "autogen_airwin/BlockParty.h"
int BlockParty_unused = AirwinRegistry::registerAirwindow({"BlockParty", "Dynamics", 167, "BlockParty is like a moderately saturated analog limiter.", airwin2rack::BlockParty::kNumParameters, []() { return std::make_unique<airwin2rack::BlockParty::BlockParty>(0); }});
#include "autogen_airwin/BrassRider.h"
int BrassRider_unused = AirwinRegistry::registerAirwindow({"BrassRider", "Dynamics", 173, "BrassRider is One Weird Trick for drum overheads in metal!", airwin2rack::BrassRider::kNumParameters, []() { return std::make_unique<airwin2rack::BrassRider::BrassRider>(0); }});
#include "autogen_airwin/BrightAmbience.h"
int BrightAmbience_unused = AirwinRegistry::registerAirwindow({"BrightAmbience", "Ambience", 19, "BrightAmbience is a plugin for very bright gated reverbs.", airwin2rack::BrightAmbience::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience::BrightAmbience>(0); }});
#include "autogen_airwin/BrightAmbience2.h"
int BrightAmbience2_unused = AirwinRegistry::registerAirwindow({"BrightAmbience2", "Ambience", 18, "BrightAmbience2 is more BrightAmbience with better tone and more slapbacky effects.", airwin2rack::BrightAmbience2::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience2::BrightAmbience2>(0); }});
#include "autogen_airwin/BrightAmbience3.h"
int BrightAmbience3_unused = AirwinRegistry::registerAirwindow({"BrightAmbience3", "Ambience", 5, "BrightAmbience3 adds undersampling for high sample rates, and better feedback.", airwin2rack::BrightAmbience3::kNumParameters, []() { return std::make_unique<airwin2rack::BrightAmbience3::BrightAmbience3>(0); }});
#include "autogen_airwin/BuildATPDF.h"
int BuildATPDF_unused = AirwinRegistry::registerAirwindow({"BuildATPDF", "Dithers", 146, "BuildATPDF is a dither-making toolkit.", airwin2rack::BuildATPDF::kNumParameters, []() { return std::make_unique<airwin2rack::BuildATPDF::BuildATPDF>(0); }});
#include "autogen_airwin/BussColors4.h"
int BussColors4_unused = AirwinRegistry::registerAirwindow({"BussColors4", "Tone Color", 319, "BussColors4 is the Airwindows console emulations, now working up to 192K correctly.", airwin2rack::BussColors4::kNumParameters, []() { return std::make_unique<airwin2rack::BussColors4::BussColors4>(0); }});
#include "autogen_airwin/ButterComp.h"
int ButterComp_unused = AirwinRegistry::registerAirwindow({"ButterComp", "Dynamics", 172, "ButterComp is my softest, smoothest compressor.", airwin2rack::ButterComp::kNumParameters, []() { return std::make_unique<airwin2rack::ButterComp::ButterComp>(0); }});
#include "autogen_airwin/ButterComp2.h"
int ButterComp2_unused = AirwinRegistry::registerAirwindow({"ButterComp2", "Dynamics", 163, "ButterComp2 is improved ButterComp with an output control and sound upgrades.", airwin2rack::ButterComp2::kNumParameters, []() { return std::make_unique<airwin2rack::ButterComp2::ButterComp2>(0); }});
#include "autogen_airwin/C5RawBuss.h"
int C5RawBuss_unused = AirwinRegistry::registerAirwindow({"C5RawBuss", "Consoles", 114, "C5RawBuss is the original Console5 algorithm, with optional very gentle DC suppression.", airwin2rack::C5RawBuss::kNumParameters, []() { return std::make_unique<airwin2rack::C5RawBuss::C5RawBuss>(0); }});
#include "autogen_airwin/C5RawChannel.h"
int C5RawChannel_unused = AirwinRegistry::registerAirwindow({"C5RawChannel", "Consoles", 115, "C5RawChannel is the original Console5 algorithm, with optional very gentle DC suppression.", airwin2rack::C5RawChannel::kNumParameters, []() { return std::make_unique<airwin2rack::C5RawChannel::C5RawChannel>(0); }});
#include "autogen_airwin/CStrip.h"
int CStrip_unused = AirwinRegistry::registerAirwindow({"CStrip", "Unclassified", -1, "CStrip is an Airwindows channel strip.", airwin2rack::CStrip::kNumParameters, []() { return std::make_unique<airwin2rack::CStrip::CStrip>(0); }});
#include "autogen_airwin/CStrip2.h"
int CStrip2_unused = AirwinRegistry::registerAirwindow({"CStrip2", "Filter", 210, "CStrip2 refines and optimizes CStrip, by request!", airwin2rack::CStrip2::kNumParameters, []() { return std::make_unique<airwin2rack::CStrip2::CStrip2>(0); }});
#include "autogen_airwin/Cabs.h"
int Cabs_unused = AirwinRegistry::registerAirwindow({"Cabs", "Amp Sims", 32, "Cabs is an Airwindows guitar speaker cabinet simulator.", airwin2rack::Cabs::kNumParameters, []() { return std::make_unique<airwin2rack::Cabs::Cabs>(0); }});
#include "autogen_airwin/Calibre.h"
int Calibre_unused = AirwinRegistry::registerAirwindow({"Calibre", "Tone Color", 324, "Calibre is a re-release of another old Character plugin.", airwin2rack::Calibre::kNumParameters, []() { return std::make_unique<airwin2rack::Calibre::Calibre>(0); }});
#include "autogen_airwin/Capacitor.h"
int Capacitor_unused = AirwinRegistry::registerAirwindow({"Capacitor", "Filter", 223, "Capacitor is a lowpass/highpass filter of a new type.", airwin2rack::Capacitor::kNumParameters, []() { return std::make_unique<airwin2rack::Capacitor::Capacitor>(0); }});
#include "autogen_airwin/Capacitor2.h"
int Capacitor2_unused = AirwinRegistry::registerAirwindow({"Capacitor2", "Filter", 206, "Capacitor2 is Capacitor with extra analog modeling and mojo.", airwin2rack::Capacitor2::kNumParameters, []() { return std::make_unique<airwin2rack::Capacitor2::Capacitor2>(0); }});
#include "autogen_airwin/Chamber.h"
int Chamber_unused = AirwinRegistry::registerAirwindow({"Chamber", "Reverb", 265, "Chamber is a feedforward reverb based on the golden ratio.", airwin2rack::Chamber::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber::Chamber>(0); }});
#include "autogen_airwin/Chamber2.h"
int Chamber2_unused = AirwinRegistry::registerAirwindow({"Chamber2", "Reverb", 264, "Chamber2 is a feedforward reverb, a blur delay, and a glitch topping!", airwin2rack::Chamber2::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber2::Chamber2>(0); }});
#include "autogen_airwin/Channel4.h"
int Channel4_unused = AirwinRegistry::registerAirwindow({"Channel4", "Tone Color", 333, "Channel4 is a tone coloring plugin that gives more of an analog feel.", airwin2rack::Channel4::kNumParameters, []() { return std::make_unique<airwin2rack::Channel4::Channel4>(0); }});
#include "autogen_airwin/Channel5.h"
int Channel5_unused = AirwinRegistry::registerAirwindow({"Channel5", "Tone Color", 332, "Channel5 is Channel for 2018, with new subtleties.", airwin2rack::Channel5::kNumParameters, []() { return std::make_unique<airwin2rack::Channel5::Channel5>(0); }});
#include "autogen_airwin/Channel6.h"
int Channel6_unused = AirwinRegistry::registerAirwindow({"Channel6", "Tone Color", 331, "Channel6 uses the Spiral algorithm.", airwin2rack::Channel6::kNumParameters, []() { return std::make_unique<airwin2rack::Channel6::Channel6>(0); }});
#include "autogen_airwin/Channel7.h"
int Channel7_unused = AirwinRegistry::registerAirwindow({"Channel7", "Tone Color", 330, "Channel7 improves slew clipping and lets you blend Channel 6 and 5.", airwin2rack::Channel7::kNumParameters, []() { return std::make_unique<airwin2rack::Channel7::Channel7>(0); }});
#include "autogen_airwin/Channel8.h"
int Channel8_unused = AirwinRegistry::registerAirwindow({"Channel8", "Tone Color", 329, "Channel8 is Channel7 with updates from Capacitor2 and Slew3. More and better of what Channel is.", airwin2rack::Channel8::kNumParameters, []() { return std::make_unique<airwin2rack::Channel8::Channel8>(0); }});
#include "autogen_airwin/Channel9.h"
int Channel9_unused = AirwinRegistry::registerAirwindow({"Channel9", "Tone Color", 320, "Channel9 is Channel8 with ultrasonic filtering, and new Teac and Mackie 'low end' settings.", airwin2rack::Channel9::kNumParameters, []() { return std::make_unique<airwin2rack::Channel9::Channel9>(0); }});
#include "autogen_airwin/Chorus.h"
int Chorus_unused = AirwinRegistry::registerAirwindow({"Chorus", "Ambience", 22, "Chorus is a mono chorus, also works as a vibrato.", airwin2rack::Chorus::kNumParameters, []() { return std::make_unique<airwin2rack::Chorus::Chorus>(0); }});
#include "autogen_airwin/ChorusEnsemble.h"
int ChorusEnsemble_unused = AirwinRegistry::registerAirwindow({"ChorusEnsemble", "Ambience", 23, "ChorusEnsemble is a more complex, multi-tap mono chorus.", airwin2rack::ChorusEnsemble::kNumParameters, []() { return std::make_unique<airwin2rack::ChorusEnsemble::ChorusEnsemble>(0); }});
#include "autogen_airwin/ChromeOxide.h"
int ChromeOxide_unused = AirwinRegistry::registerAirwindow({"ChromeOxide", "Lo-Fi", 241, "ChromeOxide is an alternate path to vibey old tape sonics.", airwin2rack::ChromeOxide::kNumParameters, []() { return std::make_unique<airwin2rack::ChromeOxide::ChromeOxide>(0); }});
#include "autogen_airwin/Cider.h"
int Cider_unused = AirwinRegistry::registerAirwindow({"Cider", "Tone Color", 325, "Cider is a re-release of another old Character plugin.", airwin2rack::Cider::kNumParameters, []() { return std::make_unique<airwin2rack::Cider::Cider>(0); }});
#include "autogen_airwin/ClearCoat.h"
int ClearCoat_unused = AirwinRegistry::registerAirwindow({"ClearCoat", "Ambience", 0, "ClearCoat is an array of bright ambience reverbs.", airwin2rack::ClearCoat::kNumParameters, []() { return std::make_unique<airwin2rack::ClearCoat::ClearCoat>(0); }});
#include "autogen_airwin/ClipOnly2.h"
int ClipOnly2_unused = AirwinRegistry::registerAirwindow({"ClipOnly2", "Clipping", 72, "ClipOnly2 suppresses the brightness of digital clipping without affecting unclipped samples, at any sample rate.", airwin2rack::ClipOnly2::kNumParameters, []() { return std::make_unique<airwin2rack::ClipOnly2::ClipOnly2>(0); }});
#include "autogen_airwin/ClipSoftly.h"
int ClipSoftly_unused = AirwinRegistry::registerAirwindow({"ClipSoftly", "Clipping", 73, "ClipSoftly is ClipOnly2 but as a softclipper: a very handy building block.", airwin2rack::ClipSoftly::kNumParameters, []() { return std::make_unique<airwin2rack::ClipSoftly::ClipSoftly>(0); }});
#include "autogen_airwin/CloudCoat.h"
int CloudCoat_unused = AirwinRegistry::registerAirwindow({"CloudCoat", "Effects", 184, "CloudCoat is an array of blur effects with a taste for evil.", airwin2rack::CloudCoat::kNumParameters, []() { return std::make_unique<airwin2rack::CloudCoat::CloudCoat>(0); }});
#include "autogen_airwin/Coils.h"
int Coils_unused = AirwinRegistry::registerAirwindow({"Coils", "Subtlety", 308, "Coils models the types of distortion you'll find in transformers.", airwin2rack::Coils::kNumParameters, []() { return std::make_unique<airwin2rack::Coils::Coils>(0); }});
#include "autogen_airwin/Coils2.h"
int Coils2_unused = AirwinRegistry::registerAirwindow({"Coils2", "Subtlety", 303, "Coils2 is a transformer overdrive emulator.", airwin2rack::Coils2::kNumParameters, []() { return std::make_unique<airwin2rack::Coils2::Coils2>(0); }});
#include "autogen_airwin/Cojones.h"
int Cojones_unused = AirwinRegistry::registerAirwindow({"Cojones", "Lo-Fi", 242, "Cojones is a new kind of distorty.", airwin2rack::Cojones::kNumParameters, []() { return std::make_unique<airwin2rack::Cojones::Cojones>(0); }});
#include "autogen_airwin/Compresaturator.h"
int Compresaturator_unused = AirwinRegistry::registerAirwindow({"Compresaturator", "Dynamics", 170, "Compresaturator fades between compressing and soft clipping.", airwin2rack::Compresaturator::kNumParameters, []() { return std::make_unique<airwin2rack::Compresaturator::Compresaturator>(0); }});
#include "autogen_airwin/Console0Buss.h"
int Console0Buss_unused = AirwinRegistry::registerAirwindow({"Console0Buss", "Consoles", 96, "Console0Buss is a radical, minimalist Console system.", airwin2rack::Console0Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console0Buss::Console0Buss>(0); }});
#include "autogen_airwin/Console0Channel.h"
int Console0Channel_unused = AirwinRegistry::registerAirwindow({"Console0Channel", "Consoles", 95, "Console0Channel is a radical, minimalist Console system.", airwin2rack::Console0Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console0Channel::Console0Channel>(0); }});
#include "autogen_airwin/Console4Buss.h"
int Console4Buss_unused = AirwinRegistry::registerAirwindow({"Console4Buss", "Consoles", 118, "Console4Buss is the original Airwindows ITB mixing system, a two-plugin mix buss replacement for DAWs.", airwin2rack::Console4Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console4Buss::Console4Buss>(0); }});
#include "autogen_airwin/Console4Channel.h"
int Console4Channel_unused = AirwinRegistry::registerAirwindow({"Console4Channel", "Consoles", 119, "Console4Channel is the original Airwindows ITB mixing system, a two-plugin mix buss replacement for DAWs.", airwin2rack::Console4Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console4Channel::Console4Channel>(0); }});
#include "autogen_airwin/Console5Buss.h"
int Console5Buss_unused = AirwinRegistry::registerAirwindow({"Console5Buss", "Consoles", 111, "Console5Buss is a richer, warmer Console system.", airwin2rack::Console5Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console5Buss::Console5Buss>(0); }});
#include "autogen_airwin/Console5Channel.h"
int Console5Channel_unused = AirwinRegistry::registerAirwindow({"Console5Channel", "Consoles", 112, "Console5Channel is a richer, warmer Console system.", airwin2rack::Console5Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console5Channel::Console5Channel>(0); }});
#include "autogen_airwin/Console5DarkCh.h"
int Console5DarkCh_unused = AirwinRegistry::registerAirwindow({"Console5DarkCh", "Consoles", 113, "Console5DarkCh is a richer, warmer Console system.", airwin2rack::Console5DarkCh::kNumParameters, []() { return std::make_unique<airwin2rack::Console5DarkCh::Console5DarkCh>(0); }});
#include "autogen_airwin/Console6Buss.h"
int Console6Buss_unused = AirwinRegistry::registerAirwindow({"Console6Buss", "Consoles", 107, "Console6Buss is a new more intense Console that peaks at lower dB.", airwin2rack::Console6Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console6Buss::Console6Buss>(0); }});
#include "autogen_airwin/Console6Channel.h"
int Console6Channel_unused = AirwinRegistry::registerAirwindow({"Console6Channel", "Consoles", 108, "Console6Channel is a new more intense Console that peaks at lower dB.", airwin2rack::Console6Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console6Channel::Console6Channel>(0); }});
#include "autogen_airwin/Console7Buss.h"
int Console7Buss_unused = AirwinRegistry::registerAirwindow({"Console7Buss", "Consoles", 101, "Console7Buss adds anti-alias filtering and special saturation curves.", airwin2rack::Console7Buss::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Buss::Console7Buss>(0); }});
#include "autogen_airwin/Console7Cascade.h"
int Console7Cascade_unused = AirwinRegistry::registerAirwindow({"Console7Cascade", "Consoles", 103, "Console7Cascade is a drop-in replacement for Console7Channel that allows for MUCH higher gain.", airwin2rack::Console7Cascade::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Cascade::Console7Cascade>(0); }});
#include "autogen_airwin/Console7Channel.h"
int Console7Channel_unused = AirwinRegistry::registerAirwindow({"Console7Channel", "Consoles", 102, "Console7Channel adds anti-alias filtering and special saturation curves.", airwin2rack::Console7Channel::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Channel::Console7Channel>(0); }});
#include "autogen_airwin/Console7Crunch.h"
int Console7Crunch_unused = AirwinRegistry::registerAirwindow({"Console7Crunch", "Consoles", 104, "Console7Crunch shows you Cascade but with its EQ weighted for more edge and grit.", airwin2rack::Console7Crunch::kNumParameters, []() { return std::make_unique<airwin2rack::Console7Crunch::Console7Crunch>(0); }});
#include "autogen_airwin/Console8BussHype.h"
int Console8BussHype_unused = AirwinRegistry::registerAirwindow({"Console8BussHype", "Consoles", 86, "Console8BussHype swaps out just the In section of Console8BussIn, for a brighter sparklier circuit.", airwin2rack::Console8BussHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussHype::Console8BussHype>(0); }});
#include "autogen_airwin/Console8BussIn.h"
int Console8BussIn_unused = AirwinRegistry::registerAirwindow({"Console8BussIn", "Consoles", 87, "Console8BussIn moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8BussIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussIn::Console8BussIn>(0); }});
#include "autogen_airwin/Console8BussOut.h"
int Console8BussOut_unused = AirwinRegistry::registerAirwindow({"Console8BussOut", "Consoles", 88, "Console8BussOut moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8BussOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8BussOut::Console8BussOut>(0); }});
#include "autogen_airwin/Console8ChannelHype.h"
int Console8ChannelHype_unused = AirwinRegistry::registerAirwindow({"Console8ChannelHype", "Consoles", 89, "Console8ChannelHype swaps out just the In section of Console8ChannelIn, for a brighter sparklier circuit.", airwin2rack::Console8ChannelHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelHype::Console8ChannelHype>(0); }});
#include "autogen_airwin/Console8ChannelIn.h"
int Console8ChannelIn_unused = AirwinRegistry::registerAirwindow({"Console8ChannelIn", "Consoles", 90, "Console8ChannelIn moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8ChannelIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelIn::Console8ChannelIn>(0); }});
#include "autogen_airwin/Console8ChannelOut.h"
int Console8ChannelOut_unused = AirwinRegistry::registerAirwindow({"Console8ChannelOut", "Consoles", 91, "Console8ChannelOut moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8ChannelOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8ChannelOut::Console8ChannelOut>(0); }});
#include "autogen_airwin/Console8LiteBuss.h"
int Console8LiteBuss_unused = AirwinRegistry::registerAirwindow({"Console8LiteBuss", "Consoles", 85, "Console8LiteBuss is simplified Console8, working with just a single mix buss.", airwin2rack::Console8LiteBuss::kNumParameters, []() { return std::make_unique<airwin2rack::Console8LiteBuss::Console8LiteBuss>(0); }});
#include "autogen_airwin/Console8LiteChannel.h"
int Console8LiteChannel_unused = AirwinRegistry::registerAirwindow({"Console8LiteChannel", "Consoles", 84, "Console8LiteChannel is simplified Console8, working with just a single mix buss.", airwin2rack::Console8LiteChannel::kNumParameters, []() { return std::make_unique<airwin2rack::Console8LiteChannel::Console8LiteChannel>(0); }});
#include "autogen_airwin/Console8SubHype.h"
int Console8SubHype_unused = AirwinRegistry::registerAirwindow({"Console8SubHype", "Consoles", 92, "Console8SubHype swaps out just the In section of Console8SubIn, for a brighter sparklier circuit.", airwin2rack::Console8SubHype::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubHype::Console8SubHype>(0); }});
#include "autogen_airwin/Console8SubIn.h"
int Console8SubIn_unused = AirwinRegistry::registerAirwindow({"Console8SubIn", "Consoles", 93, "Console8SubIn moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8SubIn::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubIn::Console8SubIn>(0); }});
#include "autogen_airwin/Console8SubOut.h"
int Console8SubOut_unused = AirwinRegistry::registerAirwindow({"Console8SubOut", "Consoles", 94, "Console8SubOut moves to a channel/submix/buss topology and adds analog simulation.", airwin2rack::Console8SubOut::kNumParameters, []() { return std::make_unique<airwin2rack::Console8SubOut::Console8SubOut>(0); }});
#include "autogen_airwin/ConsoleLABuss.h"
int ConsoleLABuss_unused = AirwinRegistry::registerAirwindow({"ConsoleLABuss", "Consoles", 78, "ConsoleLABuss is the Airwindows take on the Quad Eight console.", airwin2rack::ConsoleLABuss::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleLABuss::ConsoleLABuss>(0); }});
#include "autogen_airwin/ConsoleLAChannel.h"
int ConsoleLAChannel_unused = AirwinRegistry::registerAirwindow({"ConsoleLAChannel", "Consoles", 79, "ConsoleLAChannel is the Airwindows take on the Quad Eight console.", airwin2rack::ConsoleLAChannel::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleLAChannel::ConsoleLAChannel>(0); }});
#include "autogen_airwin/ConsoleMCBuss.h"
int ConsoleMCBuss_unused = AirwinRegistry::registerAirwindow({"ConsoleMCBuss", "Consoles", 80, "ConsoleMCBuss is the initial, bright take on the MCI console.", airwin2rack::ConsoleMCBuss::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleMCBuss::ConsoleMCBuss>(0); }});
#include "autogen_airwin/ConsoleMCChannel.h"
int ConsoleMCChannel_unused = AirwinRegistry::registerAirwindow({"ConsoleMCChannel", "Consoles", 81, "ConsoleMCChannel is the initial, bright take on the MCI console.", airwin2rack::ConsoleMCChannel::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleMCChannel::ConsoleMCChannel>(0); }});
#include "autogen_airwin/ConsoleMDBuss.h"
int ConsoleMDBuss_unused = AirwinRegistry::registerAirwindow({"ConsoleMDBuss", "Consoles", 82, "ConsoleMDBuss goes for the tone shaping of the classic MCI console!", airwin2rack::ConsoleMDBuss::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleMDBuss::ConsoleMDBuss>(0); }});
#include "autogen_airwin/ConsoleMDChannel.h"
int ConsoleMDChannel_unused = AirwinRegistry::registerAirwindow({"ConsoleMDChannel", "Consoles", 83, "ConsoleMDChannel goes for the tone shaping of the classic MCI console!", airwin2rack::ConsoleMDChannel::kNumParameters, []() { return std::make_unique<airwin2rack::ConsoleMDChannel::ConsoleMDChannel>(0); }});
#include "autogen_airwin/Creature.h"
int Creature_unused = AirwinRegistry::registerAirwindow({"Creature", "Saturation", 273, "Creature is a soft slew saturator, a new class of noisechanger.", airwin2rack::Creature::kNumParameters, []() { return std::make_unique<airwin2rack::Creature::Creature>(0); }});
#include "autogen_airwin/CrickBass.h"
int CrickBass_unused = AirwinRegistry::registerAirwindow({"CrickBass", "Amp Sims", 28, "CrickBass is a flexible but aggressive bass tone for dual pickups.", airwin2rack::CrickBass::kNumParameters, []() { return std::make_unique<airwin2rack::CrickBass::CrickBass>(0); }});
#include "autogen_airwin/CrunchyGrooveWear.h"
int CrunchyGrooveWear_unused = AirwinRegistry::registerAirwindow({"CrunchyGrooveWear", "Lo-Fi", 235, "CrunchyGrooveWear is a version of GrooveWear for more edge and distortion.", airwin2rack::CrunchyGrooveWear::kNumParameters, []() { return std::make_unique<airwin2rack::CrunchyGrooveWear::CrunchyGrooveWear>(0); }});
#include "autogen_airwin/Crystal.h"
int Crystal_unused = AirwinRegistry::registerAirwindow({"Crystal", "Tone Color", 326, "Crystal is a tone shaper and buss soft-clipper.", airwin2rack::Crystal::kNumParameters, []() { return std::make_unique<airwin2rack::Crystal::Crystal>(0); }});
#include "autogen_airwin/DCVoltage.h"
int DCVoltage_unused = AirwinRegistry::registerAirwindow({"DCVoltage", "Utility", 347, "DCVoltage is literally a DC control voltage, in a plugin.", airwin2rack::DCVoltage::kNumParameters, []() { return std::make_unique<airwin2rack::DCVoltage::DCVoltage>(0); }});
#include "autogen_airwin/Dark.h"
int Dark_unused = AirwinRegistry::registerAirwindow({"Dark", "Dithers", 133, "Dark is a wordlength reducer that gives your music a blacker backdrop.", airwin2rack::Dark::kNumParameters, []() { return std::make_unique<airwin2rack::Dark::Dark>(0); }});
#include "autogen_airwin/DarkNoise.h"
int DarkNoise_unused = AirwinRegistry::registerAirwindow({"DarkNoise", "Noise", 251, "DarkNoise is an alternative method for producing bassier noise directly. Sound design stuff.", airwin2rack::DarkNoise::kNumParameters, []() { return std::make_unique<airwin2rack::DarkNoise::DarkNoise>(0); }});
#include "autogen_airwin/DeBess.h"
int DeBess_unused = AirwinRegistry::registerAirwindow({"DeBess", "Brightness", 50, "DeBess is an improved DeEss, with perfect rejection of non-ess audio.", airwin2rack::DeBess::kNumParameters, []() { return std::make_unique<airwin2rack::DeBess::DeBess>(0); }});
#include "autogen_airwin/DeEss.h"
int DeEss_unused = AirwinRegistry::registerAirwindow({"DeEss", "Brightness", 55, "DeEss is the best de-essing solution there is. A go-to utility plugin.", airwin2rack::DeEss::kNumParameters, []() { return std::make_unique<airwin2rack::DeEss::DeEss>(0); }});
#include "autogen_airwin/DeHiss.h"
int DeHiss_unused = AirwinRegistry::registerAirwindow({"DeHiss", "Brightness", 65, "DeHiss tries to suppress background hiss, like a hiss gate.", airwin2rack::DeHiss::kNumParameters, []() { return std::make_unique<airwin2rack::DeHiss::DeHiss>(0); }});
#include "autogen_airwin/DeRez.h"
int DeRez_unused = AirwinRegistry::registerAirwindow({"DeRez", "Lo-Fi", 240, "DeRez is an analog-style bit and sample rate crusher with continuous adjustments.", airwin2rack::DeRez::kNumParameters, []() { return std::make_unique<airwin2rack::DeRez::DeRez>(0); }});
#include "autogen_airwin/DeRez2.h"
int DeRez2_unused = AirwinRegistry::registerAirwindow({"DeRez2", "Lo-Fi", 238, "DeRez2 brings more authentic retro-digital hardware tones.", airwin2rack::DeRez2::kNumParameters, []() { return std::make_unique<airwin2rack::DeRez2::DeRez2>(0); }});
#include "autogen_airwin/Deckwrecka.h"
int Deckwrecka_unused = AirwinRegistry::registerAirwindow({"Deckwrecka", "Lo-Fi", 245, "Deckwrecka fattens and dirties up beats.", airwin2rack::Deckwrecka::kNumParameters, []() { return std::make_unique<airwin2rack::Deckwrecka::Deckwrecka>(0); }});
#include "autogen_airwin/Density.h"
int Density_unused = AirwinRegistry::registerAirwindow({"Density", "Distortion", 127, "Density is a Swiss Army Knife of saturation/antisaturation.", airwin2rack::Density::kNumParameters, []() { return std::make_unique<airwin2rack::Density::Density>(0); }});
#include "autogen_airwin/Density2.h"
int Density2_unused = AirwinRegistry::registerAirwindow({"Density2", "Distortion", 126, "Density2 is a different color for Density, some old code I had that people wanted.", airwin2rack::Density2::kNumParameters, []() { return std::make_unique<airwin2rack::Density2::Density2>(0); }});
#include "autogen_airwin/Desk.h"
int Desk_unused = AirwinRegistry::registerAirwindow({"Desk", "Subtlety", 309, "Desk is classic Airwindows subtle analog modeling.", airwin2rack::Desk::kNumParameters, []() { return std::make_unique<airwin2rack::Desk::Desk>(0); }});
#include "autogen_airwin/Desk4.h"
int Desk4_unused = AirwinRegistry::registerAirwindow({"Desk4", "Saturation", 285, "Desk4 is distinctive analog coloration (a tuneable version of the control-less Desk plugins)", airwin2rack::Desk4::kNumParameters, []() { return std::make_unique<airwin2rack::Desk4::Desk4>(0); }});
#include "autogen_airwin/DigitalBlack.h"
int DigitalBlack_unused = AirwinRegistry::registerAirwindow({"DigitalBlack", "Dynamics", 160, "DigitalBlack is a quick, staccato gate.", airwin2rack::DigitalBlack::kNumParameters, []() { return std::make_unique<airwin2rack::DigitalBlack::DigitalBlack>(0); }});
#include "autogen_airwin/Dirt.h"
int Dirt_unused = AirwinRegistry::registerAirwindow({"Dirt", "Distortion", 123, "Dirt is a soft-clip distortion in the spirit of Edge.", airwin2rack::Dirt::kNumParameters, []() { return std::make_unique<airwin2rack::Dirt::Dirt>(0); }});
#include "autogen_airwin/Discontinuity.h"
int Discontinuity_unused = AirwinRegistry::registerAirwindow({"Discontinuity", "Unclassified", -1, "", airwin2rack::Discontinuity::kNumParameters, []() { return std::make_unique<airwin2rack::Discontinuity::Discontinuity>(0); }});
#include "autogen_airwin/Distance.h"
int Distance_unused = AirwinRegistry::registerAirwindow({"Distance", "Filter", 230, "Distance is a sound design or reverb far-away-izer.", airwin2rack::Distance::kNumParameters, []() { return std::make_unique<airwin2rack::Distance::Distance>(0); }});
#include "autogen_airwin/Distance2.h"
int Distance2_unused = AirwinRegistry::registerAirwindow({"Distance2", "Filter", 229, "Distance2 is a versatile space shaper for creating depth.", airwin2rack::Distance2::kNumParameters, []() { return std::make_unique<airwin2rack::Distance2::Distance2>(0); }});
#include "autogen_airwin/Distortion.h"
int Distortion_unused = AirwinRegistry::registerAirwindow({"Distortion", "Distortion", 121, "Distortion is a slightly dark analog-style distortion with several presets, like Focus.", airwin2rack::Distortion::kNumParameters, []() { return std::make_unique<airwin2rack::Distortion::Distortion>(0); }});
#include "autogen_airwin/DitherFloat.h"
int DitherFloat_unused = AirwinRegistry::registerAirwindow({"DitherFloat", "Dithers", 155, "DitherFloat is a demo of floating point truncation: can also dither to 32 bit float output.", airwin2rack::DitherFloat::kNumParameters, []() { return std::make_unique<airwin2rack::DitherFloat::DitherFloat>(0); }});
#include "autogen_airwin/DitherMeDiskers.h"
int DitherMeDiskers_unused = AirwinRegistry::registerAirwindow({"DitherMeDiskers", "Dithers", 150, "DitherMeDiskers is dark deep 'dither' in a 16 bit version.", airwin2rack::DitherMeDiskers::kNumParameters, []() { return std::make_unique<airwin2rack::DitherMeDiskers::DitherMeDiskers>(0); }});
#include "autogen_airwin/DitherMeTimbers.h"
int DitherMeTimbers_unused = AirwinRegistry::registerAirwindow({"DitherMeTimbers", "Dithers", 149, "DitherMeTimbers is dark deep 'dither' in a 24 bit version.", airwin2rack::DitherMeTimbers::kNumParameters, []() { return std::make_unique<airwin2rack::DitherMeTimbers::DitherMeTimbers>(0); }});
#include "autogen_airwin/Ditherbox.h"
int Ditherbox_unused = AirwinRegistry::registerAirwindow({"Ditherbox", "Dithers", 145, "Ditherbox is a switchable selection of dithers in 16 and 24 bit, plus monitoring tools.", airwin2rack::Ditherbox::kNumParameters, []() { return std::make_unique<airwin2rack::Ditherbox::Ditherbox>(0); }});
#include "autogen_airwin/DoublePaul.h"
int DoublePaul_unused = AirwinRegistry::registerAirwindow({"DoublePaul", "Dithers", 144, "DoublePaul is like PaulDither but more so.", airwin2rack::DoublePaul::kNumParameters, []() { return std::make_unique<airwin2rack::DoublePaul::DoublePaul>(0); }});
#include "autogen_airwin/Doublelay.h"
int Doublelay_unused = AirwinRegistry::registerAirwindow({"Doublelay", "Ambience", 2, "Doublelay is StereoDoubler with independent delays and feedback.", airwin2rack::Doublelay::kNumParameters, []() { return std::make_unique<airwin2rack::Doublelay::Doublelay>(0); }});
#include "autogen_airwin/Drive.h"
int Drive_unused = AirwinRegistry::registerAirwindow({"Drive", "Distortion", 128, "Drive is the angry distortion!", airwin2rack::Drive::kNumParameters, []() { return std::make_unique<airwin2rack::Drive::Drive>(0); }});
#include "autogen_airwin/DrumSlam.h"
int DrumSlam_unused = AirwinRegistry::registerAirwindow({"DrumSlam", "Dynamics", 171, "DrumSlam is a heavy-processing tape modeler.", airwin2rack::DrumSlam::kNumParameters, []() { return std::make_unique<airwin2rack::DrumSlam::DrumSlam>(0); }});
#include "autogen_airwin/DubCenter.h"
int DubCenter_unused = AirwinRegistry::registerAirwindow({"DubCenter", "Bass", 38, "DubCenter is a version of DubSub where the bass reinforcement is purely mono.", airwin2rack::DubCenter::kNumParameters, []() { return std::make_unique<airwin2rack::DubCenter::DubCenter>(0); }});
#include "autogen_airwin/DubSub.h"
int DubSub_unused = AirwinRegistry::registerAirwindow({"DubSub", "Bass", 39, "DubSub is a fully featured bass doctor.", airwin2rack::DubSub::kNumParameters, []() { return std::make_unique<airwin2rack::DubSub::DubSub>(0); }});
#include "autogen_airwin/Dubly.h"
int Dubly_unused = AirwinRegistry::registerAirwindow({"Dubly", "Effects", 186, "Dubly is a retro vibe and atmosphere maker.", airwin2rack::Dubly::kNumParameters, []() { return std::make_unique<airwin2rack::Dubly::Dubly>(0); }});
#include "autogen_airwin/DustBunny.h"
int DustBunny_unused = AirwinRegistry::registerAirwindow({"DustBunny", "Lo-Fi", 246, "DustBunny is unpredictable distorty noise, like dust wedged under your turntable needle.", airwin2rack::DustBunny::kNumParameters, []() { return std::make_unique<airwin2rack::DustBunny::DustBunny>(0); }});
#include "autogen_airwin/Dynamics.h"
int Dynamics_unused = AirwinRegistry::registerAirwindow({"Dynamics", "Dynamics", 158, "Dynamics is a combination compressor gate.", airwin2rack::Dynamics::kNumParameters, []() { return std::make_unique<airwin2rack::Dynamics::Dynamics>(0); }});
#include "autogen_airwin/Dyno.h"
int Dyno_unused = AirwinRegistry::registerAirwindow({"Dyno", "Saturation", 282, "Dyno is a lot like Mojo, but for intensity instead of loudness.", airwin2rack::Dyno::kNumParameters, []() { return std::make_unique<airwin2rack::Dyno::Dyno>(0); }});
#include "autogen_airwin/EQ.h"
int EQ_unused = AirwinRegistry::registerAirwindow({"EQ", "Filter", 222, "EQ is just the EQ parts of CStrip.", airwin2rack::EQ::kNumParameters, []() { return std::make_unique<airwin2rack::EQ::EQ>(0); }});
#include "autogen_airwin/EdIsDim.h"
int EdIsDim_unused = AirwinRegistry::registerAirwindow({"EdIsDim", "Utility", 348, "EdIsDim is mid/side conversion utility plugins.", airwin2rack::EdIsDim::kNumParameters, []() { return std::make_unique<airwin2rack::EdIsDim::EdIsDim>(0); }});
#include "autogen_airwin/Edge.h"
int Edge_unused = AirwinRegistry::registerAirwindow({"Edge", "Distortion", 122, "Edge is a seven-stage distortion with Hypersonic filtering and tone shaping controls.", airwin2rack::Edge::kNumParameters, []() { return std::make_unique<airwin2rack::Edge::Edge>(0); }});
#include "autogen_airwin/Elation.h"
int Elation_unused = AirwinRegistry::registerAirwindow({"Elation", "Tone Color", 323, "Elation is a re-release of another old Character plugin.", airwin2rack::Elation::kNumParameters, []() { return std::make_unique<airwin2rack::Elation::Elation>(0); }});
#include "autogen_airwin/ElectroHat.h"
int ElectroHat_unused = AirwinRegistry::registerAirwindow({"ElectroHat", "Noise", 252, "ElectroHat is a hi-hat tone generator that uses the original sound as a control voltage.", airwin2rack::ElectroHat::kNumParameters, []() { return std::make_unique<airwin2rack::ElectroHat::ElectroHat>(0); }});
#include "autogen_airwin/Energy.h"
int Energy_unused = AirwinRegistry::registerAirwindow({"Energy", "Effects", 198, "Energy is electrifying fixed-frequency treble boosts.", airwin2rack::Energy::kNumParameters, []() { return std::make_unique<airwin2rack::Energy::Energy>(0); }});
#include "autogen_airwin/Energy2.h"
int Energy2_unused = AirwinRegistry::registerAirwindow({"Energy2", "Effects", 197, "Energy2 is electrifying fixed-frequency treble boosts for high sample rate.", airwin2rack::Energy2::kNumParameters, []() { return std::make_unique<airwin2rack::Energy2::Energy2>(0); }});
#include "autogen_airwin/Ensemble.h"
int Ensemble_unused = AirwinRegistry::registerAirwindow({"Ensemble", "Ambience", 24, "Ensemble is a weird flangey little modulation effect.", airwin2rack::Ensemble::kNumParameters, []() { return std::make_unique<airwin2rack::Ensemble::Ensemble>(0); }});
#include "autogen_airwin/EveryConsole.h"
int EveryConsole_unused = AirwinRegistry::registerAirwindow({"EveryConsole", "Consoles", 120, "EveryConsole is mix-and-match Airwindows Console.", airwin2rack::EveryConsole::kNumParameters, []() { return std::make_unique<airwin2rack::EveryConsole::EveryConsole>(0); }});
#include "autogen_airwin/EverySlew.h"
int EverySlew_unused = AirwinRegistry::registerAirwindow({"EverySlew", "Brightness", 57, "EverySlew is a wide variety of unique sound processing tools.", airwin2rack::EverySlew::kNumParameters, []() { return std::make_unique<airwin2rack::EverySlew::EverySlew>(0); }});
#include "autogen_airwin/EveryTrim.h"
int EveryTrim_unused = AirwinRegistry::registerAirwindow({"EveryTrim", "Utility", 341, "EveryTrim is Left/Right, Mid/Side, and Master in one plugin.", airwin2rack::EveryTrim::kNumParameters, []() { return std::make_unique<airwin2rack::EveryTrim::EveryTrim>(0); }});
#include "autogen_airwin/Exciter.h"
int Exciter_unused = AirwinRegistry::registerAirwindow({"Exciter", "Effects", 196, "Exciter is an Aural Exciter plugin that can be both subtle and extreme.", airwin2rack::Exciter::kNumParameters, []() { return std::make_unique<airwin2rack::Exciter::Exciter>(0); }});
#include "autogen_airwin/Facet.h"
int Facet_unused = AirwinRegistry::registerAirwindow({"Facet", "Effects", 199, "Facet is a new kind of hard clip that adds two sharp corners into the wave.", airwin2rack::Facet::kNumParameters, []() { return std::make_unique<airwin2rack::Facet::Facet>(0); }});
#include "autogen_airwin/FathomFive.h"
int FathomFive_unused = AirwinRegistry::registerAirwindow({"FathomFive", "Bass", 42, "FathomFive is a way of supplementing extreme bass that’s not just EQ.", airwin2rack::FathomFive::kNumParameters, []() { return std::make_unique<airwin2rack::FathomFive::FathomFive>(0); }});
#include "autogen_airwin/FinalClip.h"
int FinalClip_unused = AirwinRegistry::registerAirwindow({"FinalClip", "Unclassified", -1, "", airwin2rack::FinalClip::kNumParameters, []() { return std::make_unique<airwin2rack::FinalClip::FinalClip>(0); }});
#include "autogen_airwin/FireAmp.h"
int FireAmp_unused = AirwinRegistry::registerAirwindow({"FireAmp", "Amp Sims", 26, "FireAmp is a bright, loud, tubey amp sim for leads and dirt guitar.", airwin2rack::FireAmp::kNumParameters, []() { return std::make_unique<airwin2rack::FireAmp::FireAmp>(0); }});
#include "autogen_airwin/Flipity.h"
int Flipity_unused = AirwinRegistry::registerAirwindow({"Flipity", "Utility", 355, "Flipity is a channel phase flipper/swapper utility.", airwin2rack::Flipity::kNumParameters, []() { return std::make_unique<airwin2rack::Flipity::Flipity>(0); }});
#include "autogen_airwin/Floor.h"
int Floor_unused = AirwinRegistry::registerAirwindow({"Floor", "Bass", 40, "Floor is fake bottom octave for fun and profit!", airwin2rack::Floor::kNumParameters, []() { return std::make_unique<airwin2rack::Floor::Floor>(0); }});
#include "autogen_airwin/Flutter.h"
int Flutter_unused = AirwinRegistry::registerAirwindow({"Flutter", "Lo-Fi", 234, "Flutter is the most recent Airwindows flutter, standalone.", airwin2rack::Flutter::kNumParameters, []() { return std::make_unique<airwin2rack::Flutter::Flutter>(0); }});
#include "autogen_airwin/Focus.h"
int Focus_unused = AirwinRegistry::registerAirwindow({"Focus", "Saturation", 280, "Focus brings out clarity by distorting. Aggressive, subtle, flexible.", airwin2rack::Focus::kNumParameters, []() { return std::make_unique<airwin2rack::Focus::Focus>(0); }});
#include "autogen_airwin/Fracture.h"
int Fracture_unused = AirwinRegistry::registerAirwindow({"Fracture", "Effects", 200, "Fracture is a frequency multiplier/waveshaper with a soft disruption to the sound.", airwin2rack::Fracture::kNumParameters, []() { return std::make_unique<airwin2rack::Fracture::Fracture>(0); }});
#include "autogen_airwin/Fracture2.h"
int Fracture2_unused = AirwinRegistry::registerAirwindow({"Fracture2", "Effects", 185, "Fracture2 is a wavefolder that allows more extreme disruption.", airwin2rack::Fracture2::kNumParameters, []() { return std::make_unique<airwin2rack::Fracture2::Fracture2>(0); }});
#include "autogen_airwin/FromTape.h"
int FromTape_unused = AirwinRegistry::registerAirwindow({"FromTape", "Tape", 313, "FromTape is a minimalist, cleaner analog tape emulation.", airwin2rack::FromTape::kNumParameters, []() { return std::make_unique<airwin2rack::FromTape::FromTape>(0); }});
#include "autogen_airwin/Galactic.h"
int Galactic_unused = AirwinRegistry::registerAirwindow({"Galactic", "Reverb", 261, "Galactic is a super-reverb designed specially for pads and space ambient.", airwin2rack::Galactic::kNumParameters, []() { return std::make_unique<airwin2rack::Galactic::Galactic>(0); }});
#include "autogen_airwin/Galactic2.h"
int Galactic2_unused = AirwinRegistry::registerAirwindow({"Galactic2", "Reverb", 262, "Galactic2 is a super-reverb REdesigned specially for pads and space ambient.", airwin2rack::Galactic2::kNumParameters, []() { return std::make_unique<airwin2rack::Galactic2::Galactic2>(0); }});
#include "autogen_airwin/GalacticVibe.h"
int GalacticVibe_unused = AirwinRegistry::registerAirwindow({"GalacticVibe", "Effects", 183, "GalacticVibe is the stereo vibrato from the original Galactic reverb.", airwin2rack::GalacticVibe::kNumParameters, []() { return std::make_unique<airwin2rack::GalacticVibe::GalacticVibe>(0); }});
#include "autogen_airwin/Gatelope.h"
int Gatelope_unused = AirwinRegistry::registerAirwindow({"Gatelope", "Dynamics", 175, "Gatelope is a special gate that applies filters.", airwin2rack::Gatelope::kNumParameters, []() { return std::make_unique<airwin2rack::Gatelope::Gatelope>(0); }});
#include "autogen_airwin/GlitchShifter.h"
int GlitchShifter_unused = AirwinRegistry::registerAirwindow({"GlitchShifter", "Effects", 194, "GlitchShifter is a really gnarly, raw-sounding pitch shifter with a dose of insanity!", airwin2rack::GlitchShifter::kNumParameters, []() { return std::make_unique<airwin2rack::GlitchShifter::GlitchShifter>(0); }});
#include "autogen_airwin/GoldenSlew.h"
int GoldenSlew_unused = AirwinRegistry::registerAirwindow({"GoldenSlew", "Brightness", 51, "GoldenSlew is a multistage slew clipper based on the Golden Ratio.", airwin2rack::GoldenSlew::kNumParameters, []() { return std::make_unique<airwin2rack::GoldenSlew::GoldenSlew>(0); }});
#include "autogen_airwin/Golem.h"
int Golem_unused = AirwinRegistry::registerAirwindow({"Golem", "Utility", 346, "Golem lets you blend a stereo track of two mics on an amp.", airwin2rack::Golem::kNumParameters, []() { return std::make_unique<airwin2rack::Golem::Golem>(0); }});
#include "autogen_airwin/GrindAmp.h"
int GrindAmp_unused = AirwinRegistry::registerAirwindow({"GrindAmp", "Amp Sims", 25, "GrindAmp is a heavier amp sim for many purposes.", airwin2rack::GrindAmp::kNumParameters, []() { return std::make_unique<airwin2rack::GrindAmp::GrindAmp>(0); }});
#include "autogen_airwin/Gringer.h"
int Gringer_unused = AirwinRegistry::registerAirwindow({"Gringer", "Effects", 195, "Gringer is a full-wave rectifier plugin, like a Green Ringer guitar effect.", airwin2rack::Gringer::kNumParameters, []() { return std::make_unique<airwin2rack::Gringer::Gringer>(0); }});
#include "autogen_airwin/GrooveWear.h"
int GrooveWear_unused = AirwinRegistry::registerAirwindow({"GrooveWear", "Lo-Fi", 236, "GrooveWear is for scrubbing highs off mechanically like a stylus would.", airwin2rack::GrooveWear::kNumParameters, []() { return std::make_unique<airwin2rack::GrooveWear::GrooveWear>(0); }});
#include "autogen_airwin/GuitarConditioner.h"
int GuitarConditioner_unused = AirwinRegistry::registerAirwindow({"GuitarConditioner", "Effects", 190, "GuitarConditioner is like a Tube Screamer voicing without the squishiness and indistinctness.", airwin2rack::GuitarConditioner::kNumParameters, []() { return std::make_unique<airwin2rack::GuitarConditioner::GuitarConditioner>(0); }});
#include "autogen_airwin/HardVacuum.h"
int HardVacuum_unused = AirwinRegistry::registerAirwindow({"HardVacuum", "Distortion", 130, "HardVacuum is tube style saturation effects.", airwin2rack::HardVacuum::kNumParameters, []() { return std::make_unique<airwin2rack::HardVacuum::HardVacuum>(0); }});
#include "autogen_airwin/HermeTrim.h"
int HermeTrim_unused = AirwinRegistry::registerAirwindow({"HermeTrim", "Utility", 342, "HermeTrim is Very Fine Adjustments, otherwise just like EveryTrim.", airwin2rack::HermeTrim::kNumParameters, []() { return std::make_unique<airwin2rack::HermeTrim::HermeTrim>(0); }});
#include "autogen_airwin/Hermepass.h"
int Hermepass_unused = AirwinRegistry::registerAirwindow({"Hermepass", "Bass", 36, "Hermepass is a mastering highpass to set by ear only.", airwin2rack::Hermepass::kNumParameters, []() { return std::make_unique<airwin2rack::Hermepass::Hermepass>(0); }});
#include "autogen_airwin/HighGlossDither.h"
int HighGlossDither_unused = AirwinRegistry::registerAirwindow({"HighGlossDither", "Dithers", 154, "HighGlossDither is a hybrid between shiny dither and truncation! Unnatural science experiment.", airwin2rack::HighGlossDither::kNumParameters, []() { return std::make_unique<airwin2rack::HighGlossDither::HighGlossDither>(0); }});
#include "autogen_airwin/HighImpact.h"
int HighImpact_unused = AirwinRegistry::registerAirwindow({"HighImpact", "Distortion", 131, "HighImpact is distorted grit and punch without fatness.", airwin2rack::HighImpact::kNumParameters, []() { return std::make_unique<airwin2rack::HighImpact::HighImpact>(0); }});
#include "autogen_airwin/Highpass.h"
int Highpass_unused = AirwinRegistry::registerAirwindow({"Highpass", "Filter", 232, "Highpass is a time warp, for retro midrangey sounds.", airwin2rack::Highpass::kNumParameters, []() { return std::make_unique<airwin2rack::Highpass::Highpass>(0); }});
#include "autogen_airwin/Highpass2.h"
int Highpass2_unused = AirwinRegistry::registerAirwindow({"Highpass2", "Filter", 228, "Highpass2 is an unusual-sounding variable-slope highpass filter.", airwin2rack::Highpass2::kNumParameters, []() { return std::make_unique<airwin2rack::Highpass2::Highpass2>(0); }});
#include "autogen_airwin/Holt.h"
int Holt_unused = AirwinRegistry::registerAirwindow({"Holt", "Filter", 214, "Holt is a synth-like resonant lowpass filter focussed on bass frequencies.", airwin2rack::Holt::kNumParameters, []() { return std::make_unique<airwin2rack::Holt::Holt>(0); }});
#include "autogen_airwin/Holt2.h"
int Holt2_unused = AirwinRegistry::registerAirwindow({"Holt2", "Filter", 213, "Holt2 is a bass-cab-like resonant lowpass filter.", airwin2rack::Holt2::kNumParameters, []() { return std::make_unique<airwin2rack::Holt2::Holt2>(0); }});
#include "autogen_airwin/Hombre.h"
int Hombre_unused = AirwinRegistry::registerAirwindow({"Hombre", "Ambience", 17, "Hombre is atmosphere and texture (through very short delays).", airwin2rack::Hombre::kNumParameters, []() { return std::make_unique<airwin2rack::Hombre::Hombre>(0); }});
#include "autogen_airwin/Huge.h"
int Huge_unused = AirwinRegistry::registerAirwindow({"Huge", "Saturation", 274, "Huge is a dirty, dirty loudenator.", airwin2rack::Huge::kNumParameters, []() { return std::make_unique<airwin2rack::Huge::Huge>(0); }});
#include "autogen_airwin/Hull.h"
int Hull_unused = AirwinRegistry::registerAirwindow({"Hull", "Filter", 221, "Hull is an alternate form of highpass/lowpass filter.", airwin2rack::Hull::kNumParameters, []() { return std::make_unique<airwin2rack::Hull::Hull>(0); }});
#include "autogen_airwin/Hull2.h"
int Hull2_unused = AirwinRegistry::registerAirwindow({"Hull2", "Filter", 219, "Hull2 is a very clear three-band EQ.", airwin2rack::Hull2::kNumParameters, []() { return std::make_unique<airwin2rack::Hull2::Hull2>(0); }});
#include "autogen_airwin/Hype.h"
int Hype_unused = AirwinRegistry::registerAirwindow({"Hype", "Subtlety", 297, "Hype is a treble softening experiment gone strangely awry :)", airwin2rack::Hype::kNumParameters, []() { return std::make_unique<airwin2rack::Hype::Hype>(0); }});
#include "autogen_airwin/HypersonX.h"
int HypersonX_unused = AirwinRegistry::registerAirwindow({"HypersonX", "Brightness", 67, "HypersonX is like UltrasonX but with seven stages instead of five.", airwin2rack::HypersonX::kNumParameters, []() { return std::make_unique<airwin2rack::HypersonX::HypersonX>(0); }});
#include "autogen_airwin/Hypersonic.h"
int Hypersonic_unused = AirwinRegistry::registerAirwindow({"Hypersonic", "Brightness", 66, "Hypersonic is Ultrasonic, but steeper and higher cutoff.", airwin2rack::Hypersonic::kNumParameters, []() { return std::make_unique<airwin2rack::Hypersonic::Hypersonic>(0); }});
#include "autogen_airwin/Infinity.h"
int Infinity_unused = AirwinRegistry::registerAirwindow({"Infinity", "Reverb", 269, "Infinity is a MatrixVerb specifically designed to sustain and layer sounds forever.", airwin2rack::Infinity::kNumParameters, []() { return std::make_unique<airwin2rack::Infinity::Infinity>(0); }});
#include "autogen_airwin/Infinity2.h"
int Infinity2_unused = AirwinRegistry::registerAirwindow({"Infinity2", "Reverb", 266, "Infinity2 expands Infinity with more tone shaping and a much-requested kill-switch!", airwin2rack::Infinity2::kNumParameters, []() { return std::make_unique<airwin2rack::Infinity2::Infinity2>(0); }});
#include "autogen_airwin/Inflamer.h"
int Inflamer_unused = AirwinRegistry::registerAirwindow({"Inflamer", "Subtlety", 299, "Inflamer is an Airwindowsized take on the waveshapers in Oxford Inflator.", airwin2rack::Inflamer::kNumParameters, []() { return std::make_unique<airwin2rack::Inflamer::Inflamer>(0); }});
#include "autogen_airwin/Infrasonic.h"
int Infrasonic_unused = AirwinRegistry::registerAirwindow({"Infrasonic", "Bass", 41, "Infrasonic is a very steep subsonic filter, built like Ultrasonic.", airwin2rack::Infrasonic::kNumParameters, []() { return std::make_unique<airwin2rack::Infrasonic::Infrasonic>(0); }});
#include "autogen_airwin/Interstage.h"
int Interstage_unused = AirwinRegistry::registerAirwindow({"Interstage", "Subtlety", 304, "Interstage is a subtle and sophisticated analogifier.", airwin2rack::Interstage::kNumParameters, []() { return std::make_unique<airwin2rack::Interstage::Interstage>(0); }});
#include "autogen_airwin/IronOxide5.h"
int IronOxide5_unused = AirwinRegistry::registerAirwindow({"IronOxide5", "Tape", 316, "IronOxide5 is the old school, heavily colored Airwindows tape emulation.", airwin2rack::IronOxide5::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxide5::IronOxide5>(0); }});
#include "autogen_airwin/IronOxideClassic.h"
int IronOxideClassic_unused = AirwinRegistry::registerAirwindow({"IronOxideClassic", "Tape", 318, "IronOxideClassic is the purer, simpler, early form of Iron Oxide before all the features.", airwin2rack::IronOxideClassic::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic::IronOxideClassic>(0); }});
#include "autogen_airwin/IronOxideClassic2.h"
int IronOxideClassic2_unused = AirwinRegistry::registerAirwindow({"IronOxideClassic2", "Tape", 315, "IronOxideClassic2 is my bandpassy tape sim, updated for high sample rate and aliasing control.", airwin2rack::IronOxideClassic2::kNumParameters, []() { return std::make_unique<airwin2rack::IronOxideClassic2::IronOxideClassic2>(0); }});
#include "autogen_airwin/Isolator.h"
int Isolator_unused = AirwinRegistry::registerAirwindow({"Isolator", "Filter", 224, "Isolator is a steep highpass or lowpass filter, like you might find in a speaker crossover.", airwin2rack::Isolator::kNumParameters, []() { return std::make_unique<airwin2rack::Isolator::Isolator>(0); }});
#include "autogen_airwin/Isolator2.h"
int Isolator2_unused = AirwinRegistry::registerAirwindow({"Isolator2", "Filter", 212, "Isolator2 is Isolator, but with control smoothing and a new resonance booster.", airwin2rack::Isolator2::kNumParameters, []() { return std::make_unique<airwin2rack::Isolator2::Isolator2>(0); }});
#include "autogen_airwin/Kalman.h"
int Kalman_unused = AirwinRegistry::registerAirwindow({"Kalman", "Unclassified", -1, "", airwin2rack::Kalman::kNumParameters, []() { return std::make_unique<airwin2rack::Kalman::Kalman>(0); }});
#include "autogen_airwin/LRFlipTimer.h"
int LRFlipTimer_unused = AirwinRegistry::registerAirwindow({"LRFlipTimer", "Stereo", 293, "LRFlipTimer is a utility that swaps Left with Right every few (1-10) minutes.", airwin2rack::LRFlipTimer::kNumParameters, []() { return std::make_unique<airwin2rack::LRFlipTimer::LRFlipTimer>(0); }});
#include "autogen_airwin/LeadAmp.h"
int LeadAmp_unused = AirwinRegistry::registerAirwindow({"LeadAmp", "Amp Sims", 27, "LeadAmp is an amp sim with a clear, penetrating, vocal tone.", airwin2rack::LeadAmp::kNumParameters, []() { return std::make_unique<airwin2rack::LeadAmp::LeadAmp>(0); }});
#include "autogen_airwin/LeftoMono.h"
int LeftoMono_unused = AirwinRegistry::registerAirwindow({"LeftoMono", "Utility", 353, "LeftoMono copies one channel to both, losslessly.", airwin2rack::LeftoMono::kNumParameters, []() { return std::make_unique<airwin2rack::LeftoMono::LeftoMono>(0); }});
#include "autogen_airwin/LilAmp.h"
int LilAmp_unused = AirwinRegistry::registerAirwindow({"LilAmp", "Amp Sims", 29, "LilAmp is a tiny amp sim, like a little bitty practice amp without that much gain.", airwin2rack::LilAmp::kNumParameters, []() { return std::make_unique<airwin2rack::LilAmp::LilAmp>(0); }});
#include "autogen_airwin/Logical4.h"
int Logical4_unused = AirwinRegistry::registerAirwindow({"Logical4", "Dynamics", 161, "Logical4 is a classic 2-buss compressor.", airwin2rack::Logical4::kNumParameters, []() { return std::make_unique<airwin2rack::Logical4::Logical4>(0); }});
#include "autogen_airwin/Loud.h"
int Loud_unused = AirwinRegistry::registerAirwindow({"Loud", "Distortion", 129, "Loud is distortion and demolition of air molecules, modeled.", airwin2rack::Loud::kNumParameters, []() { return std::make_unique<airwin2rack::Loud::Loud>(0); }});
#include "autogen_airwin/Lowpass.h"
int Lowpass_unused = AirwinRegistry::registerAirwindow({"Lowpass", "Filter", 231, "Lowpass deepens the tone, leaves a gloss and textural modifications.", airwin2rack::Lowpass::kNumParameters, []() { return std::make_unique<airwin2rack::Lowpass::Lowpass>(0); }});
#include "autogen_airwin/Lowpass2.h"
int Lowpass2_unused = AirwinRegistry::registerAirwindow({"Lowpass2", "Filter", 227, "Lowpass2 is an unusual-sounding variable-slope lowpass filter.", airwin2rack::Lowpass2::kNumParameters, []() { return std::make_unique<airwin2rack::Lowpass2::Lowpass2>(0); }});
#include "autogen_airwin/Luxor.h"
int Luxor_unused = AirwinRegistry::registerAirwindow({"Luxor", "Tone Color", 328, "Luxor is a re-release of another old Character plugin.", airwin2rack::Luxor::kNumParameters, []() { return std::make_unique<airwin2rack::Luxor::Luxor>(0); }});
#include "autogen_airwin/MSFlipTimer.h"
int MSFlipTimer_unused = AirwinRegistry::registerAirwindow({"MSFlipTimer", "Stereo", 294, "MSFlipTimer is a utility that swaps stereo with mono every few (1-10) minutes.", airwin2rack::MSFlipTimer::kNumParameters, []() { return std::make_unique<airwin2rack::MSFlipTimer::MSFlipTimer>(0); }});
#include "autogen_airwin/MV.h"
int MV_unused = AirwinRegistry::registerAirwindow({"MV", "Ambience", 9, "MV is a dual-mono reverb based on BitShiftGain and the old Midiverbs.", airwin2rack::MV::kNumParameters, []() { return std::make_unique<airwin2rack::MV::MV>(0); }});
#include "autogen_airwin/MV2.h"
int MV2_unused = AirwinRegistry::registerAirwindow({"MV2", "Ambience", 8, "MV2 is a dual-mono reverb based on BitShiftGain and the old Midiverbs, adapted to high sample rates.", airwin2rack::MV2::kNumParameters, []() { return std::make_unique<airwin2rack::MV2::MV2>(0); }});
#include "autogen_airwin/MackEQ.h"
int MackEQ_unused = AirwinRegistry::registerAirwindow({"MackEQ", "Filter", 218, "MackEQ is the Mackie distortion but with treble and bass controls added.", airwin2rack::MackEQ::kNumParameters, []() { return std::make_unique<airwin2rack::MackEQ::MackEQ>(0); }});
#include "autogen_airwin/Mackity.h"
int Mackity_unused = AirwinRegistry::registerAirwindow({"Mackity", "Distortion", 124, "Mackity is an emulation of the input stage of a vintage Mackie 1202!", airwin2rack::Mackity::kNumParameters, []() { return std::make_unique<airwin2rack::Mackity::Mackity>(0); }});
#include "autogen_airwin/MatrixVerb.h"
int MatrixVerb_unused = AirwinRegistry::registerAirwindow({"MatrixVerb", "Reverb", 270, "MatrixVerb is a wildly adjustable, strangely colorful reverb for deep and flexible spaces.", airwin2rack::MatrixVerb::kNumParameters, []() { return std::make_unique<airwin2rack::MatrixVerb::MatrixVerb>(0); }});
#include "autogen_airwin/Melt.h"
int Melt_unused = AirwinRegistry::registerAirwindow({"Melt", "Ambience", 7, "Melt is a wobbly chorusy weird diffuse effect.", airwin2rack::Melt::kNumParameters, []() { return std::make_unique<airwin2rack::Melt::Melt>(0); }});
#include "autogen_airwin/MidAmp.h"
int MidAmp_unused = AirwinRegistry::registerAirwindow({"MidAmp", "Amp Sims", 30, "MidAmp is a clean amp sim meant to work like a loud Twin or something of that nature.", airwin2rack::MidAmp::kNumParameters, []() { return std::make_unique<airwin2rack::MidAmp::MidAmp>(0); }});
#include "autogen_airwin/MidSide.h"
int MidSide_unused = AirwinRegistry::registerAirwindow({"MidSide", "Utility", 349, "MidSide is the other side of the EdIsDim mid/side conversion utility plugin.", airwin2rack::MidSide::kNumParameters, []() { return std::make_unique<airwin2rack::MidSide::MidSide>(0); }});
#include "autogen_airwin/MoNoam.h"
int MoNoam_unused = AirwinRegistry::registerAirwindow({"MoNoam", "Utility", 356, "MoNoam gives you variations on mono, mid, and side in different channels.", airwin2rack::MoNoam::kNumParameters, []() { return std::make_unique<airwin2rack::MoNoam::MoNoam>(0); }});
#include "autogen_airwin/Mojo.h"
int Mojo_unused = AirwinRegistry::registerAirwindow({"Mojo", "Saturation", 281, "Mojo is a biggenator that also works as a loudenator.", airwin2rack::Mojo::kNumParameters, []() { return std::make_unique<airwin2rack::Mojo::Mojo>(0); }});
#include "autogen_airwin/Monitoring.h"
int Monitoring_unused = AirwinRegistry::registerAirwindow({"Monitoring", "Utility", 336, "Monitoring is your one-stop shop for final 2-buss work!", airwin2rack::Monitoring::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring::Monitoring>(0); }});
#include "autogen_airwin/Monitoring2.h"
int Monitoring2_unused = AirwinRegistry::registerAirwindow({"Monitoring2", "Utility", 335, "Monitoring2 is Monitoring, but with Dark as the wordlength reducer.", airwin2rack::Monitoring2::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring2::Monitoring2>(0); }});
#include "autogen_airwin/Monitoring3.h"
int Monitoring3_unused = AirwinRegistry::registerAirwindow({"Monitoring3", "Utility", 334, "Monitoring3 is Monitoring, but with Ten Nines into Dark as the wordlength reducer.", airwin2rack::Monitoring3::kNumParameters, []() { return std::make_unique<airwin2rack::Monitoring3::Monitoring3>(0); }});
#include "autogen_airwin/MultiBandDistortion.h"
int MultiBandDistortion_unused = AirwinRegistry::registerAirwindow({"MultiBandDistortion", "Distortion", 132, "MultiBandDistortion is an old weird gnarly sound wrecker :)", airwin2rack::MultiBandDistortion::kNumParameters, []() { return std::make_unique<airwin2rack::MultiBandDistortion::MultiBandDistortion>(0); }});
#include "autogen_airwin/NCSeventeen.h"
int NCSeventeen_unused = AirwinRegistry::registerAirwindow({"NCSeventeen", "Saturation", 275, "NCSeventeen is Dirty Loud!", airwin2rack::NCSeventeen::kNumParameters, []() { return std::make_unique<airwin2rack::NCSeventeen::NCSeventeen>(0); }});
#include "autogen_airwin/NaturalizeDither.h"
int NaturalizeDither_unused = AirwinRegistry::registerAirwindow({"NaturalizeDither", "Dithers", 153, "NaturalizeDither is deterministic dither that uses Benford Realness calculations for each sample.", airwin2rack::NaturalizeDither::kNumParameters, []() { return std::make_unique<airwin2rack::NaturalizeDither::NaturalizeDither>(0); }});
#include "autogen_airwin/Neverland.h"
int Neverland_unused = AirwinRegistry::registerAirwindow({"Neverland", "Tone Color", 322, "Neverland is a re-release of my old Neve-style color adder, exacly as it was.", airwin2rack::Neverland::kNumParameters, []() { return std::make_unique<airwin2rack::Neverland::Neverland>(0); }});
#include "autogen_airwin/Nikola.h"
int Nikola_unused = AirwinRegistry::registerAirwindow({"Nikola", "Effects", 204, "Nikola is an Audio Tesla Coil plugin! Note: audio tesla coils don't sound nice.", airwin2rack::Nikola::kNumParameters, []() { return std::make_unique<airwin2rack::Nikola::Nikola>(0); }});
#include "autogen_airwin/NodeDither.h"
int NodeDither_unused = AirwinRegistry::registerAirwindow({"NodeDither", "Dithers", 147, "NodeDither is adjusta-TPDF-dither, like a dither flanger. Can do Paul and Tape settings.", airwin2rack::NodeDither::kNumParameters, []() { return std::make_unique<airwin2rack::NodeDither::NodeDither>(0); }});
#include "autogen_airwin/Noise.h"
int Noise_unused = AirwinRegistry::registerAirwindow({"Noise", "Noise", 247, "Noise is the Airwindows deep noise oscillator, as a sound reinforcer.", airwin2rack::Noise::kNumParameters, []() { return std::make_unique<airwin2rack::Noise::Noise>(0); }});
#include "autogen_airwin/NonlinearSpace.h"
int NonlinearSpace_unused = AirwinRegistry::registerAirwindow({"NonlinearSpace", "Reverb", 267, "NonlinearSpace is a flexible reverb plugin.", airwin2rack::NonlinearSpace::kNumParameters, []() { return std::make_unique<airwin2rack::NonlinearSpace::NonlinearSpace>(0); }});
#include "autogen_airwin/NotJustAnotherCD.h"
int NotJustAnotherCD_unused = AirwinRegistry::registerAirwindow({"NotJustAnotherCD", "Dithers", 139, "NotJustAnotherCD is a next-gen wordlength reducers for 16 bit with DeRez.", airwin2rack::NotJustAnotherCD::kNumParameters, []() { return std::make_unique<airwin2rack::NotJustAnotherCD::NotJustAnotherCD>(0); }});
#include "autogen_airwin/NotJustAnotherDither.h"
int NotJustAnotherDither_unused = AirwinRegistry::registerAirwindow({"NotJustAnotherDither", "Dithers", 138, "NotJustAnotherDither is a next-gen wordlength reducers for 24 bit with DeRez.", airwin2rack::NotJustAnotherDither::kNumParameters, []() { return std::make_unique<airwin2rack::NotJustAnotherDither::NotJustAnotherDither>(0); }});
#include "autogen_airwin/OneCornerClip.h"
int OneCornerClip_unused = AirwinRegistry::registerAirwindow({"OneCornerClip", "Clipping", 74, "OneCornerClip is an ultimate full-bandwidth clipper.", airwin2rack::OneCornerClip::kNumParameters, []() { return std::make_unique<airwin2rack::OneCornerClip::OneCornerClip>(0); }});
#include "autogen_airwin/OrbitKick.h"
int OrbitKick_unused = AirwinRegistry::registerAirwindow({"OrbitKick", "Bass", 35, "OrbitKick is a bass generator and kick drum reinforcer.", airwin2rack::OrbitKick::kNumParameters, []() { return std::make_unique<airwin2rack::OrbitKick::OrbitKick>(0); }});
#include "autogen_airwin/Overheads.h"
int Overheads_unused = AirwinRegistry::registerAirwindow({"Overheads", "Unclassified", -1, "", airwin2rack::Overheads::kNumParameters, []() { return std::make_unique<airwin2rack::Overheads::Overheads>(0); }});
#include "autogen_airwin/PDBuss.h"
int PDBuss_unused = AirwinRegistry::registerAirwindow({"PDBuss", "Consoles", 109, "PDBuss is Console5 and PurestDrive, sittin’ in a tree… (i.e. both at once, on channels and buss)", airwin2rack::PDBuss::kNumParameters, []() { return std::make_unique<airwin2rack::PDBuss::PDBuss>(0); }});
#include "autogen_airwin/PDChannel.h"
int PDChannel_unused = AirwinRegistry::registerAirwindow({"PDChannel", "Consoles", 110, "PDChannel is Console5 and PurestDrive, sittin’ in a tree… (i.e. both at once, on channels and buss)", airwin2rack::PDChannel::kNumParameters, []() { return std::make_unique<airwin2rack::PDChannel::PDChannel>(0); }});
#include "autogen_airwin/Pafnuty.h"
int Pafnuty_unused = AirwinRegistry::registerAirwindow({"Pafnuty", "Unclassified", -1, "Pafnuty is a Chebyshev filter, that adds harmonics.", airwin2rack::Pafnuty::kNumParameters, []() { return std::make_unique<airwin2rack::Pafnuty::Pafnuty>(0); }});
#include "autogen_airwin/Pafnuty2.h"
int Pafnuty2_unused = AirwinRegistry::registerAirwindow({"Pafnuty2", "Effects", 187, "Pafnuty2 is a Chebyshev filter, that adds harmonics, and fits in the VCV Rack port.", airwin2rack::Pafnuty2::kNumParameters, []() { return std::make_unique<airwin2rack::Pafnuty2::Pafnuty2>(0); }});
#include "autogen_airwin/PaulDither.h"
int PaulDither_unused = AirwinRegistry::registerAirwindow({"PaulDither", "Dithers", 135, "PaulDither is a highpassed TPDF dither. (quieter, airier)", airwin2rack::PaulDither::kNumParameters, []() { return std::make_unique<airwin2rack::PaulDither::PaulDither>(0); }});
#include "autogen_airwin/PaulWide.h"
int PaulWide_unused = AirwinRegistry::registerAirwindow({"PaulWide", "Dithers", 134, "PaulWide is a highpassed TPDF wide dither. (quieter, airier AND wider)", airwin2rack::PaulWide::kNumParameters, []() { return std::make_unique<airwin2rack::PaulWide::PaulWide>(0); }});
#include "autogen_airwin/PeaksOnly.h"
int PeaksOnly_unused = AirwinRegistry::registerAirwindow({"PeaksOnly", "Utility", 345, "PeaksOnly is a transformative mix check tool.", airwin2rack::PeaksOnly::kNumParameters, []() { return std::make_unique<airwin2rack::PeaksOnly::PeaksOnly>(0); }});
#include "autogen_airwin/Pear.h"
int Pear_unused = AirwinRegistry::registerAirwindow({"Pear", "Filter", 207, "Pear is the testbed for a new filter, implemented as a highpass/lowpass shelf.", airwin2rack::Pear::kNumParameters, []() { return std::make_unique<airwin2rack::Pear::Pear>(0); }});
#include "autogen_airwin/PhaseNudge.h"
int PhaseNudge_unused = AirwinRegistry::registerAirwindow({"PhaseNudge", "Subtlety", 305, "PhaseNudge is a phase rotator/allpass filter.", airwin2rack::PhaseNudge::kNumParameters, []() { return std::make_unique<airwin2rack::PhaseNudge::PhaseNudge>(0); }});
#include "autogen_airwin/PitchDelay.h"
int PitchDelay_unused = AirwinRegistry::registerAirwindow({"PitchDelay", "Ambience", 3, "PitchDelay is TapeDelay2 but with pitch shift instead of flutter.", airwin2rack::PitchDelay::kNumParameters, []() { return std::make_unique<airwin2rack::PitchDelay::PitchDelay>(0); }});
#include "autogen_airwin/PitchNasty.h"
int PitchNasty_unused = AirwinRegistry::registerAirwindow({"PitchNasty", "Effects", 188, "PitchNasty is a primitive pitch shifter that also has primitive time-stretch artifacts.", airwin2rack::PitchNasty::kNumParameters, []() { return std::make_unique<airwin2rack::PitchNasty::PitchNasty>(0); }});
#include "autogen_airwin/PlatinumSlew.h"
int PlatinumSlew_unused = AirwinRegistry::registerAirwindow({"PlatinumSlew", "Brightness", 49, "PlatinumSlew is the best easy-to-use slew clipper.", airwin2rack::PlatinumSlew::kNumParameters, []() { return std::make_unique<airwin2rack::PlatinumSlew::PlatinumSlew>(0); }});
#include "autogen_airwin/PocketVerbs.h"
int PocketVerbs_unused = AirwinRegistry::registerAirwindow({"PocketVerbs", "Reverb", 271, "PocketVerbs is my popular old special effects reverbs plugin!", airwin2rack::PocketVerbs::kNumParameters, []() { return std::make_unique<airwin2rack::PocketVerbs::PocketVerbs>(0); }});
#include "autogen_airwin/Pockey.h"
int Pockey_unused = AirwinRegistry::registerAirwindow({"Pockey", "Lo-Fi", 237, "Pockey is 12 bit (and under) lo-fi hiphop in a plugin.", airwin2rack::Pockey::kNumParameters, []() { return std::make_unique<airwin2rack::Pockey::Pockey>(0); }});
#include "autogen_airwin/Pockey2.h"
int Pockey2_unused = AirwinRegistry::registerAirwindow({"Pockey2", "Lo-Fi", 233, "Pockey2 is more efficient, more intense lo-fi hiphop in a plugin.", airwin2rack::Pockey2::kNumParameters, []() { return std::make_unique<airwin2rack::Pockey2::Pockey2>(0); }});
#include "autogen_airwin/Podcast.h"
int Podcast_unused = AirwinRegistry::registerAirwindow({"Podcast", "Dynamics", 177, "Podcast is a simpler pile of curve-style compressors with hard clipping.", airwin2rack::Podcast::kNumParameters, []() { return std::make_unique<airwin2rack::Podcast::Podcast>(0); }});
#include "autogen_airwin/PodcastDeluxe.h"
int PodcastDeluxe_unused = AirwinRegistry::registerAirwindow({"PodcastDeluxe", "Dynamics", 176, "PodcastDeluxe is a pile of compressors (curve style) and phase rotators.", airwin2rack::PodcastDeluxe::kNumParameters, []() { return std::make_unique<airwin2rack::PodcastDeluxe::PodcastDeluxe>(0); }});
#include "autogen_airwin/Point.h"
int Point_unused = AirwinRegistry::registerAirwindow({"Point", "Dynamics", 174, "Point is an explosive transient designer.", airwin2rack::Point::kNumParameters, []() { return std::make_unique<airwin2rack::Point::Point>(0); }});
#include "autogen_airwin/Pop.h"
int Pop_unused = AirwinRegistry::registerAirwindow({"Pop", "Dynamics", 159, "Pop is a crazy overcompressor with a very exaggerated sound.", airwin2rack::Pop::kNumParameters, []() { return std::make_unique<airwin2rack::Pop::Pop>(0); }});
#include "autogen_airwin/Pop2.h"
int Pop2_unused = AirwinRegistry::registerAirwindow({"Pop2", "Dynamics", 156, "Pop2 adds control and punch to Pop.", airwin2rack::Pop2::kNumParameters, []() { return std::make_unique<airwin2rack::Pop2::Pop2>(0); }});
#include "autogen_airwin/PowerSag.h"
int PowerSag_unused = AirwinRegistry::registerAirwindow({"PowerSag", "Effects", 202, "PowerSag is for emulating power supply limitations in analog modeling.", airwin2rack::PowerSag::kNumParameters, []() { return std::make_unique<airwin2rack::PowerSag::PowerSag>(0); }});
#include "autogen_airwin/PowerSag2.h"
int PowerSag2_unused = AirwinRegistry::registerAirwindow({"PowerSag2", "Effects", 201, "PowerSag2 is my improved circuit-starve plugin, now with inverse effect!", airwin2rack::PowerSag2::kNumParameters, []() { return std::make_unique<airwin2rack::PowerSag2::PowerSag2>(0); }});
#include "autogen_airwin/Precious.h"
int Precious_unused = AirwinRegistry::registerAirwindow({"Precious", "Tone Color", 327, "Precious is a re-release of another old Character plugin.", airwin2rack::Precious::kNumParameters, []() { return std::make_unique<airwin2rack::Precious::Precious>(0); }});
#include "autogen_airwin/Preponderant.h"
int Preponderant_unused = AirwinRegistry::registerAirwindow({"Preponderant", "Effects", 203, "Preponderant is Airwindows Anti-Soothe, a strange tone intensifier and balancer.", airwin2rack::Preponderant::kNumParameters, []() { return std::make_unique<airwin2rack::Preponderant::Preponderant>(0); }});
#include "autogen_airwin/Pressure4.h"
int Pressure4_unused = AirwinRegistry::registerAirwindow({"Pressure4", "Dynamics", 180, "Pressure4 is a compressor adjustable between vari-mu and ‘new york’ peak-retaining behaviors.", airwin2rack::Pressure4::kNumParameters, []() { return std::make_unique<airwin2rack::Pressure4::Pressure4>(0); }});
#include "autogen_airwin/Pressure5.h"
int Pressure5_unused = AirwinRegistry::registerAirwindow({"Pressure5", "Dynamics", 157, "Pressure5 expands Pressure4 with after-boost and built-in ClipOnly2.", airwin2rack::Pressure5::kNumParameters, []() { return std::make_unique<airwin2rack::Pressure5::Pressure5>(0); }});
#include "autogen_airwin/PurestAir.h"
int PurestAir_unused = AirwinRegistry::registerAirwindow({"PurestAir", "Brightness", 63, "PurestAir is a brightness experiment.", airwin2rack::PurestAir::kNumParameters, []() { return std::make_unique<airwin2rack::PurestAir::PurestAir>(0); }});
#include "autogen_airwin/PurestConsole2Buss.h"
int PurestConsole2Buss_unused = AirwinRegistry::registerAirwindow({"PurestConsole2Buss", "Consoles", 99, "PurestConsole2Buss is the distributed filter version of PurestConsole.", airwin2rack::PurestConsole2Buss::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole2Buss::PurestConsole2Buss>(0); }});
#include "autogen_airwin/PurestConsole2Channel.h"
int PurestConsole2Channel_unused = AirwinRegistry::registerAirwindow({"PurestConsole2Channel", "Consoles", 100, "PurestConsole2Channel is the distributed filter version of PurestConsole.", airwin2rack::PurestConsole2Channel::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole2Channel::PurestConsole2Channel>(0); }});
#include "autogen_airwin/PurestConsole3Buss.h"
int PurestConsole3Buss_unused = AirwinRegistry::registerAirwindow({"PurestConsole3Buss", "Consoles", 97, "PurestConsole3Buss is an alternate algorithm for minimalist Console.", airwin2rack::PurestConsole3Buss::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole3Buss::PurestConsole3Buss>(0); }});
#include "autogen_airwin/PurestConsole3Channel.h"
int PurestConsole3Channel_unused = AirwinRegistry::registerAirwindow({"PurestConsole3Channel", "Consoles", 98, "PurestConsole3Channel is an alternate algorithm for minimalist Console.", airwin2rack::PurestConsole3Channel::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsole3Channel::PurestConsole3Channel>(0); }});
#include "autogen_airwin/PurestConsoleBuss.h"
int PurestConsoleBuss_unused = AirwinRegistry::registerAirwindow({"PurestConsoleBuss", "Consoles", 105, "PurestConsoleBuss is the most free from coloration Console system.", airwin2rack::PurestConsoleBuss::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsoleBuss::PurestConsoleBuss>(0); }});
#include "autogen_airwin/PurestConsoleChannel.h"
int PurestConsoleChannel_unused = AirwinRegistry::registerAirwindow({"PurestConsoleChannel", "Consoles", 106, "PurestConsoleChannel is the most free from coloration Console system.", airwin2rack::PurestConsoleChannel::kNumParameters, []() { return std::make_unique<airwin2rack::PurestConsoleChannel::PurestConsoleChannel>(0); }});
#include "autogen_airwin/PurestDrive.h"
int PurestDrive_unused = AirwinRegistry::registerAirwindow({"PurestDrive", "Saturation", 279, "PurestDrive is the magic saturation plugin of subtlety and French House tone.", airwin2rack::PurestDrive::kNumParameters, []() { return std::make_unique<airwin2rack::PurestDrive::PurestDrive>(0); }});
#include "autogen_airwin/PurestEcho.h"
int PurestEcho_unused = AirwinRegistry::registerAirwindow({"PurestEcho", "Ambience", 13, "PurestEcho is optimized Airwindows echo with exactly four evenly spaced taps on tap.", airwin2rack::PurestEcho::kNumParameters, []() { return std::make_unique<airwin2rack::PurestEcho::PurestEcho>(0); }});
#include "autogen_airwin/PurestFade.h"
int PurestFade_unused = AirwinRegistry::registerAirwindow({"PurestFade", "Utility", 340, "PurestFade is just like PurestGain, but for LONG fades.", airwin2rack::PurestFade::kNumParameters, []() { return std::make_unique<airwin2rack::PurestFade::PurestFade>(0); }});
#include "autogen_airwin/PurestGain.h"
int PurestGain_unused = AirwinRegistry::registerAirwindow({"PurestGain", "Utility", 339, "PurestGain is a high-res noise shaped gain, with smoothed fader.", airwin2rack::PurestGain::kNumParameters, []() { return std::make_unique<airwin2rack::PurestGain::PurestGain>(0); }});
#include "autogen_airwin/PurestSquish.h"
int PurestSquish_unused = AirwinRegistry::registerAirwindow({"PurestSquish", "Dynamics", 179, "PurestSquish is an open-sounding compressor with bass bloom.", airwin2rack::PurestSquish::kNumParameters, []() { return std::make_unique<airwin2rack::PurestSquish::PurestSquish>(0); }});
#include "autogen_airwin/PurestWarm.h"
int PurestWarm_unused = AirwinRegistry::registerAirwindow({"PurestWarm", "Subtlety", 302, "PurestWarm is a subtle tone shaper and warmth adder.", airwin2rack::PurestWarm::kNumParameters, []() { return std::make_unique<airwin2rack::PurestWarm::PurestWarm>(0); }});
#include "autogen_airwin/PurestWarm2.h"
int PurestWarm2_unused = AirwinRegistry::registerAirwindow({"PurestWarm2", "Subtlety", 301, "PurestWarm2 adds dual dry/wet controls for each side of the waveform.", airwin2rack::PurestWarm2::kNumParameters, []() { return std::make_unique<airwin2rack::PurestWarm2::PurestWarm2>(0); }});
#include "autogen_airwin/Pyewacket.h"
int Pyewacket_unused = AirwinRegistry::registerAirwindow({"Pyewacket", "Dynamics", 166, "Pyewacket is an old school compressor for high definition transients. Adds no fatness, just energy.", airwin2rack::Pyewacket::kNumParameters, []() { return std::make_unique<airwin2rack::Pyewacket::Pyewacket>(0); }});
#include "autogen_airwin/RawGlitters.h"
int RawGlitters_unused = AirwinRegistry::registerAirwindow({"RawGlitters", "Dithers", 152, "RawGlitters is just the quantization from Dither Me Timbers… and the opposite, as a brightener.", airwin2rack::RawGlitters::kNumParameters, []() { return std::make_unique<airwin2rack::RawGlitters::RawGlitters>(0); }});
#include "autogen_airwin/RawTimbers.h"
int RawTimbers_unused = AirwinRegistry::registerAirwindow({"RawTimbers", "Dithers", 151, "RawTimbers is just the quantization from Dither Me Timbers… and the opposite, as a brightener.", airwin2rack::RawTimbers::kNumParameters, []() { return std::make_unique<airwin2rack::RawTimbers::RawTimbers>(0); }});
#include "autogen_airwin/Recurve.h"
int Recurve_unused = AirwinRegistry::registerAirwindow({"Recurve", "Dynamics", 165, "Recurve is a special buss compressor with no threshold point.", airwin2rack::Recurve::kNumParameters, []() { return std::make_unique<airwin2rack::Recurve::Recurve>(0); }});
#include "autogen_airwin/Remap.h"
int Remap_unused = AirwinRegistry::registerAirwindow({"Remap", "Subtlety", 306, "Remap puts the guts back into overloudenated audio!", airwin2rack::Remap::kNumParameters, []() { return std::make_unique<airwin2rack::Remap::Remap>(0); }});
#include "autogen_airwin/ResEQ.h"
int ResEQ_unused = AirwinRegistry::registerAirwindow({"ResEQ", "Filter", 226, "ResEQ is a bank of mostly midrange resonances.", airwin2rack::ResEQ::kNumParameters, []() { return std::make_unique<airwin2rack::ResEQ::ResEQ>(0); }});
#include "autogen_airwin/ResEQ2.h"
int ResEQ2_unused = AirwinRegistry::registerAirwindow({"ResEQ2", "Filter", 208, "ResEQ2 is a single, sharp, sonorous mid peak.", airwin2rack::ResEQ2::kNumParameters, []() { return std::make_unique<airwin2rack::ResEQ2::ResEQ2>(0); }});
#include "autogen_airwin/Reverb.h"
int Reverb_unused = AirwinRegistry::registerAirwindow({"Reverb", "Reverb", 272, "Reverb is simplified and tuned MatrixVerb, adjusting everything for each optimum verb size.", airwin2rack::Reverb::kNumParameters, []() { return std::make_unique<airwin2rack::Reverb::Reverb>(0); }});
#include "autogen_airwin/Righteous4.h"
int Righteous4_unused = AirwinRegistry::registerAirwindow({"Righteous4", "Saturation", 286, "Righteous4 is a final output stage for targeting dynamic range.", airwin2rack::Righteous4::kNumParameters, []() { return std::make_unique<airwin2rack::Righteous4::Righteous4>(0); }});
#include "autogen_airwin/RightoMono.h"
int RightoMono_unused = AirwinRegistry::registerAirwindow({"RightoMono", "Utility", 352, "RightoMono copies one channel to both, losslessly.", airwin2rack::RightoMono::kNumParameters, []() { return std::make_unique<airwin2rack::RightoMono::RightoMono>(0); }});
#include "autogen_airwin/SampleDelay.h"
int SampleDelay_unused = AirwinRegistry::registerAirwindow({"SampleDelay", "Ambience", 4, "SampleDelay is three delays combined: millisecond, sample and subsample.", airwin2rack::SampleDelay::kNumParameters, []() { return std::make_unique<airwin2rack::SampleDelay::SampleDelay>(0); }});
#include "autogen_airwin/Shape.h"
int Shape_unused = AirwinRegistry::registerAirwindow({"Shape", "Subtlety", 298, "Shape is an asymmetrical waveshaper for peak manipulating.", airwin2rack::Shape::kNumParameters, []() { return std::make_unique<airwin2rack::Shape::Shape>(0); }});
#include "autogen_airwin/ShortBuss.h"
int ShortBuss_unused = AirwinRegistry::registerAirwindow({"ShortBuss", "Effects", 189, "ShortBuss chases second harmonic, to add or remove it.", airwin2rack::ShortBuss::kNumParameters, []() { return std::make_unique<airwin2rack::ShortBuss::ShortBuss>(0); }});
#include "autogen_airwin/SideDull.h"
int SideDull_unused = AirwinRegistry::registerAirwindow({"SideDull", "Stereo", 296, "SideDull is like Sidepass, but a lowpass on the side channel. Treble-centerer.", airwin2rack::SideDull::kNumParameters, []() { return std::make_unique<airwin2rack::SideDull::SideDull>(0); }});
#include "autogen_airwin/Sidepass.h"
int Sidepass_unused = AirwinRegistry::registerAirwindow({"Sidepass", "Stereo", 295, "Sidepass is a simple utility plugin, a highpass on the side channel. Mono-maker.", airwin2rack::Sidepass::kNumParameters, []() { return std::make_unique<airwin2rack::Sidepass::Sidepass>(0); }});
#include "autogen_airwin/Silhouette.h"
int Silhouette_unused = AirwinRegistry::registerAirwindow({"Silhouette", "Noise", 253, "Silhouette replaces the sound with raw noise sculpted to match its dynamics.", airwin2rack::Silhouette::kNumParameters, []() { return std::make_unique<airwin2rack::Silhouette::Silhouette>(0); }});
#include "autogen_airwin/Sinew.h"
int Sinew_unused = AirwinRegistry::registerAirwindow({"Sinew", "Brightness", 52, "Sinew combines sines and slew clipping for a tape bias effect!", airwin2rack::Sinew::kNumParameters, []() { return std::make_unique<airwin2rack::Sinew::Sinew>(0); }});
#include "autogen_airwin/SingleEndedTriode.h"
int SingleEndedTriode_unused = AirwinRegistry::registerAirwindow({"SingleEndedTriode", "Subtlety", 307, "SingleEndedTriode is unusual analog modeling effects.", airwin2rack::SingleEndedTriode::kNumParameters, []() { return std::make_unique<airwin2rack::SingleEndedTriode::SingleEndedTriode>(0); }});
#include "autogen_airwin/Slew.h"
int Slew_unused = AirwinRegistry::registerAirwindow({"Slew", "Brightness", 60, "Slew is a slew clipper, which darkens treble in an unusual way.", airwin2rack::Slew::kNumParameters, []() { return std::make_unique<airwin2rack::Slew::Slew>(0); }});
#include "autogen_airwin/Slew2.h"
int Slew2_unused = AirwinRegistry::registerAirwindow({"Slew2", "Brightness", 59, "Slew2 works like a de-esser or acceleration limiter: controls extreme highs.", airwin2rack::Slew2::kNumParameters, []() { return std::make_unique<airwin2rack::Slew2::Slew2>(0); }});
#include "autogen_airwin/Slew3.h"
int Slew3_unused = AirwinRegistry::registerAirwindow({"Slew3", "Brightness", 58, "Slew3 is a new approach to slew clipping meant for a more analog-like darkening effect.", airwin2rack::Slew3::kNumParameters, []() { return std::make_unique<airwin2rack::Slew3::Slew3>(0); }});
#include "autogen_airwin/SlewOnly.h"
int SlewOnly_unused = AirwinRegistry::registerAirwindow({"SlewOnly", "Utility", 343, "SlewOnly is a mix check plugin that shows you only the extreme highs.", airwin2rack::SlewOnly::kNumParameters, []() { return std::make_unique<airwin2rack::SlewOnly::SlewOnly>(0); }});
#include "autogen_airwin/SlewSonic.h"
int SlewSonic_unused = AirwinRegistry::registerAirwindow({"SlewSonic", "Brightness", 53, "SlewSonic combines SlewOnly with ultrasonic filtering to solo brightness.", airwin2rack::SlewSonic::kNumParameters, []() { return std::make_unique<airwin2rack::SlewSonic::SlewSonic>(0); }});
#include "autogen_airwin/Smooth.h"
int Smooth_unused = AirwinRegistry::registerAirwindow({"Smooth", "Brightness", 56, "Smooth can tame pointy sounds or make drums explode.", airwin2rack::Smooth::kNumParameters, []() { return std::make_unique<airwin2rack::Smooth::Smooth>(0); }});
#include "autogen_airwin/SoftGate.h"
int SoftGate_unused = AirwinRegistry::registerAirwindow({"SoftGate", "Dynamics", 168, "SoftGate is a gate that can mute hiss and smooth sample tails.", airwin2rack::SoftGate::kNumParameters, []() { return std::make_unique<airwin2rack::SoftGate::SoftGate>(0); }});
#include "autogen_airwin/SpatializeDither.h"
int SpatializeDither_unused = AirwinRegistry::registerAirwindow({"SpatializeDither", "Dithers", 142, "SpatializeDither is a high-performance clarity and accuracy dither.", airwin2rack::SpatializeDither::kNumParameters, []() { return std::make_unique<airwin2rack::SpatializeDither::SpatializeDither>(0); }});
#include "autogen_airwin/Spiral.h"
int Spiral_unused = AirwinRegistry::registerAirwindow({"Spiral", "Saturation", 283, "Spiral is the new best smoothest distortion algorithm.", airwin2rack::Spiral::kNumParameters, []() { return std::make_unique<airwin2rack::Spiral::Spiral>(0); }});
#include "autogen_airwin/Spiral2.h"
int Spiral2_unused = AirwinRegistry::registerAirwindow({"Spiral2", "Saturation", 278, "Spiral2 is Spiral with controls including Presence.", airwin2rack::Spiral2::kNumParameters, []() { return std::make_unique<airwin2rack::Spiral2::Spiral2>(0); }});
#include "autogen_airwin/Srsly.h"
int Srsly_unused = AirwinRegistry::registerAirwindow({"Srsly", "Stereo", 287, "Srsly is a psychoacoustic stereo processor.", airwin2rack::Srsly::kNumParameters, []() { return std::make_unique<airwin2rack::Srsly::Srsly>(0); }});
#include "autogen_airwin/Srsly2.h"
int Srsly2_unused = AirwinRegistry::registerAirwindow({"Srsly2", "Stereo", 288, "Srsly2 is a revisit of Srsly, to make the stereo widening more extreme.", airwin2rack::Srsly2::kNumParameters, []() { return std::make_unique<airwin2rack::Srsly2::Srsly2>(0); }});
#include "autogen_airwin/StarChild.h"
int StarChild_unused = AirwinRegistry::registerAirwindow({"StarChild", "Ambience", 16, "StarChild is a weird digital ambience/echo plugin.", airwin2rack::StarChild::kNumParameters, []() { return std::make_unique<airwin2rack::StarChild::StarChild>(0); }});
#include "autogen_airwin/StarChild2.h"
int StarChild2_unused = AirwinRegistry::registerAirwindow({"StarChild2", "Ambience", 15, "StarChild2 is a weird digital ambience/echo plugin adapted to high sample rates.", airwin2rack::StarChild2::kNumParameters, []() { return std::make_unique<airwin2rack::StarChild2::StarChild2>(0); }});
#include "autogen_airwin/StereoChorus.h"
int StereoChorus_unused = AirwinRegistry::registerAirwindow({"StereoChorus", "Ambience", 12, "StereoChorus is a nice basic stereo chorus.", airwin2rack::StereoChorus::kNumParameters, []() { return std::make_unique<airwin2rack::StereoChorus::StereoChorus>(0); }});
#include "autogen_airwin/StereoDoubler.h"
int StereoDoubler_unused = AirwinRegistry::registerAirwindow({"StereoDoubler", "Ambience", 21, "StereoDoubler is like GlitchShifter optimized for pitch shift doubling and tripling.", airwin2rack::StereoDoubler::kNumParameters, []() { return std::make_unique<airwin2rack::StereoDoubler::StereoDoubler>(0); }});
#include "autogen_airwin/StereoEnsemble.h"
int StereoEnsemble_unused = AirwinRegistry::registerAirwindow({"StereoEnsemble", "Ambience", 20, "StereoEnsemble is a sort of hyperchorus blast from the past.", airwin2rack::StereoEnsemble::kNumParameters, []() { return std::make_unique<airwin2rack::StereoEnsemble::StereoEnsemble>(0); }});
#include "autogen_airwin/StereoFX.h"
int StereoFX_unused = AirwinRegistry::registerAirwindow({"StereoFX", "Stereo", 290, "StereoFX is an aggressive stereo widener.", airwin2rack::StereoFX::kNumParameters, []() { return std::make_unique<airwin2rack::StereoFX::StereoFX>(0); }});
#include "autogen_airwin/StudioTan.h"
int StudioTan_unused = AirwinRegistry::registerAirwindow({"StudioTan", "Dithers", 148, "StudioTan is all the 'non-dither' dithers, up to date and convenient.", airwin2rack::StudioTan::kNumParameters, []() { return std::make_unique<airwin2rack::StudioTan::StudioTan>(0); }});
#include "autogen_airwin/SubTight.h"
int SubTight_unused = AirwinRegistry::registerAirwindow({"SubTight", "Filter", 209, "SubTight uses a variation on the Creature algorithm to tighten sub-lows.", airwin2rack::SubTight::kNumParameters, []() { return std::make_unique<airwin2rack::SubTight::SubTight>(0); }});
#include "autogen_airwin/SubsOnly.h"
int SubsOnly_unused = AirwinRegistry::registerAirwindow({"SubsOnly", "Utility", 344, "SubsOnly is a mix check plugin that shows you only the extreme lows.", airwin2rack::SubsOnly::kNumParameters, []() { return std::make_unique<airwin2rack::SubsOnly::SubsOnly>(0); }});
#include "autogen_airwin/Surge.h"
int Surge_unused = AirwinRegistry::registerAirwindow({"Surge", "Dynamics", 181, "Surge is a compressor for accentuating beats and pulses.", airwin2rack::Surge::kNumParameters, []() { return std::make_unique<airwin2rack::Surge::Surge>(0); }});
#include "autogen_airwin/SurgeTide.h"
int SurgeTide_unused = AirwinRegistry::registerAirwindow({"SurgeTide", "Dynamics", 182, "SurgeTide is a surge and flow dynamics plugin.", airwin2rack::SurgeTide::kNumParameters, []() { return std::make_unique<airwin2rack::SurgeTide::SurgeTide>(0); }});
#include "autogen_airwin/Sweeten.h"
int Sweeten_unused = AirwinRegistry::registerAirwindow({"Sweeten", "Subtlety", 300, "Sweeten is where you can find super-clean second harmonic.", airwin2rack::Sweeten::kNumParameters, []() { return std::make_unique<airwin2rack::Sweeten::Sweeten>(0); }});
#include "autogen_airwin/Swell.h"
int Swell_unused = AirwinRegistry::registerAirwindow({"Swell", "Dynamics", 178, "Swell is Dial-an-attack, like sidechaining.", airwin2rack::Swell::kNumParameters, []() { return std::make_unique<airwin2rack::Swell::Swell>(0); }});
#include "autogen_airwin/TPDFDither.h"
int TPDFDither_unused = AirwinRegistry::registerAirwindow({"TPDFDither", "Dithers", 137, "TPDFDither is TPDF dither. With 16/24 bit output and a DeRez control.", airwin2rack::TPDFDither::kNumParameters, []() { return std::make_unique<airwin2rack::TPDFDither::TPDFDither>(0); }});
#include "autogen_airwin/TPDFWide.h"
int TPDFWide_unused = AirwinRegistry::registerAirwindow({"TPDFWide", "Dithers", 136, "TPDFWide is still TPDF dither but better and wider. With 16/24 bit output and a DeRez control.", airwin2rack::TPDFWide::kNumParameters, []() { return std::make_unique<airwin2rack::TPDFWide::TPDFWide>(0); }});
#include "autogen_airwin/Tape.h"
int Tape_unused = AirwinRegistry::registerAirwindow({"Tape", "Tape", 314, "Tape is simplified, all-purpose tape mojo: my personal jam.", airwin2rack::Tape::kNumParameters, []() { return std::make_unique<airwin2rack::Tape::Tape>(0); }});
#include "autogen_airwin/TapeDelay.h"
int TapeDelay_unused = AirwinRegistry::registerAirwindow({"TapeDelay", "Ambience", 14, "TapeDelay is an old school tape echo with pitch swerve effects.", airwin2rack::TapeDelay::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDelay::TapeDelay>(0); }});
#include "autogen_airwin/TapeDelay2.h"
int TapeDelay2_unused = AirwinRegistry::registerAirwindow({"TapeDelay2", "Ambience", 1, "TapeDelay2 is a new implementation of a flexible tape echo.", airwin2rack::TapeDelay2::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDelay2::TapeDelay2>(0); }});
#include "autogen_airwin/TapeDither.h"
int TapeDither_unused = AirwinRegistry::registerAirwindow({"TapeDither", "Dithers", 141, "TapeDither is TPDF dither with noise like reel-to-reel tape.", airwin2rack::TapeDither::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDither::TapeDither>(0); }});
#include "autogen_airwin/TapeDust.h"
int TapeDust_unused = AirwinRegistry::registerAirwindow({"TapeDust", "Noise", 254, "TapeDust is just a special treble-erode noise, a ‘slew noise’ plugin.", airwin2rack::TapeDust::kNumParameters, []() { return std::make_unique<airwin2rack::TapeDust::TapeDust>(0); }});
#include "autogen_airwin/TapeFat.h"
int TapeFat_unused = AirwinRegistry::registerAirwindow({"TapeFat", "Filter", 225, "TapeFat is the tone control from TapeDelay.", airwin2rack::TapeFat::kNumParameters, []() { return std::make_unique<airwin2rack::TapeFat::TapeFat>(0); }});
#include "autogen_airwin/Texturize.h"
int Texturize_unused = AirwinRegistry::registerAirwindow({"Texturize", "Noise", 248, "Texturize is a hidden-noise plugin for adding sonic texture to things.", airwin2rack::Texturize::kNumParameters, []() { return std::make_unique<airwin2rack::Texturize::Texturize>(0); }});
#include "autogen_airwin/TexturizeMS.h"
int TexturizeMS_unused = AirwinRegistry::registerAirwindow({"TexturizeMS", "Noise", 249, "TexturizeMS is a hidden-noise plugin for adding mid-side sonic texture to things.", airwin2rack::TexturizeMS::kNumParameters, []() { return std::make_unique<airwin2rack::TexturizeMS::TexturizeMS>(0); }});
#include "autogen_airwin/Thunder.h"
int Thunder_unused = AirwinRegistry::registerAirwindow({"Thunder", "Dynamics", 169, "Thunder is a compressor that retains or exaggerates subsonic bass when you push it.", airwin2rack::Thunder::kNumParameters, []() { return std::make_unique<airwin2rack::Thunder::Thunder>(0); }});
#include "autogen_airwin/ToTape5.h"
int ToTape5_unused = AirwinRegistry::registerAirwindow({"ToTape5", "Tape", 317, "ToTape5 is Airwindows analog tape emulation.", airwin2rack::ToTape5::kNumParameters, []() { return std::make_unique<airwin2rack::ToTape5::ToTape5>(0); }});
#include "autogen_airwin/ToTape6.h"
int ToTape6_unused = AirwinRegistry::registerAirwindow({"ToTape6", "Tape", 312, "ToTape6 is Airwindows tape emulation for 2020!", airwin2rack::ToTape6::kNumParameters, []() { return std::make_unique<airwin2rack::ToTape6::ToTape6>(0); }});
#include "autogen_airwin/ToVinyl4.h"
int ToVinyl4_unused = AirwinRegistry::registerAirwindow({"ToVinyl4", "Stereo", 291, "ToVinyl4 is a vinyl-mastering simulator bringing several vinyl-type colors.", airwin2rack::ToVinyl4::kNumParameters, []() { return std::make_unique<airwin2rack::ToVinyl4::ToVinyl4>(0); }});
#include "autogen_airwin/ToneSlant.h"
int ToneSlant_unused = AirwinRegistry::registerAirwindow({"ToneSlant", "Filter", 215, "ToneSlant is a super-transparent ‘tilt EQ’ with very low Q.", airwin2rack::ToneSlant::kNumParameters, []() { return std::make_unique<airwin2rack::ToneSlant::ToneSlant>(0); }});
#include "autogen_airwin/TransDesk.h"
int TransDesk_unused = AirwinRegistry::registerAirwindow({"TransDesk", "Subtlety", 310, "TransDesk is more of a transistory, rock desk analog modeling.", airwin2rack::TransDesk::kNumParameters, []() { return std::make_unique<airwin2rack::TransDesk::TransDesk>(0); }});
#include "autogen_airwin/TremoSquare.h"
int TremoSquare_unused = AirwinRegistry::registerAirwindow({"TremoSquare", "Effects", 192, "TremoSquare is a squarewave tremolo effect that only switches on zero crossings.", airwin2rack::TremoSquare::kNumParameters, []() { return std::make_unique<airwin2rack::TremoSquare::TremoSquare>(0); }});
#include "autogen_airwin/Tremolo.h"
int Tremolo_unused = AirwinRegistry::registerAirwindow({"Tremolo", "Effects", 193, "Tremolo is fluctuating saturation curves for a tubey tremolo.", airwin2rack::Tremolo::kNumParameters, []() { return std::make_unique<airwin2rack::Tremolo::Tremolo>(0); }});
#include "autogen_airwin/TripleSpread.h"
int TripleSpread_unused = AirwinRegistry::registerAirwindow({"TripleSpread", "Ambience", 6, "TripleSpread is a stereo tripler with extra wideness and GlitchShifter processing.", airwin2rack::TripleSpread::kNumParameters, []() { return std::make_unique<airwin2rack::TripleSpread::TripleSpread>(0); }});
#include "autogen_airwin/Tube.h"
int Tube_unused = AirwinRegistry::registerAirwindow({"Tube", "Saturation", 277, "Tube is a tube style SoundBetterIzer using a new algorithm for analog modeling!", airwin2rack::Tube::kNumParameters, []() { return std::make_unique<airwin2rack::Tube::Tube>(0); }});
#include "autogen_airwin/Tube2.h"
int Tube2_unused = AirwinRegistry::registerAirwindow({"Tube2", "Saturation", 276, "Tube2 extends Tube, with more sophisticated processing and input trim.", airwin2rack::Tube2::kNumParameters, []() { return std::make_unique<airwin2rack::Tube2::Tube2>(0); }});
#include "autogen_airwin/TubeDesk.h"
int TubeDesk_unused = AirwinRegistry::registerAirwindow({"TubeDesk", "Subtlety", 311, "TubeDesk is a tube recording console type tone coloring.", airwin2rack::TubeDesk::kNumParameters, []() { return std::make_unique<airwin2rack::TubeDesk::TubeDesk>(0); }});
#include "autogen_airwin/UltrasonX.h"
int UltrasonX_unused = AirwinRegistry::registerAirwindow({"UltrasonX", "Brightness", 71, "UltrasonX is a method for rolling your own Console-type systems with total control over your ultrasonic filtering.", airwin2rack::UltrasonX::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonX::UltrasonX>(0); }});
#include "autogen_airwin/Ultrasonic.h"
int Ultrasonic_unused = AirwinRegistry::registerAirwindow({"Ultrasonic", "Brightness", 68, "Ultrasonic is a very clean, plain, high quality supersonic filter, for using inside digital mixes.", airwin2rack::Ultrasonic::kNumParameters, []() { return std::make_unique<airwin2rack::Ultrasonic::Ultrasonic>(0); }});
#include "autogen_airwin/UltrasonicLite.h"
int UltrasonicLite_unused = AirwinRegistry::registerAirwindow({"UltrasonicLite", "Brightness", 69, "UltrasonicLite is just one stage of supersonic filter, for using inside digital mixes.", airwin2rack::UltrasonicLite::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonicLite::UltrasonicLite>(0); }});
#include "autogen_airwin/UltrasonicMed.h"
int UltrasonicMed_unused = AirwinRegistry::registerAirwindow({"UltrasonicMed", "Brightness", 70, "UltrasonicMed is two stages of supersonic filter, for using inside digital mixes.", airwin2rack::UltrasonicMed::kNumParameters, []() { return std::make_unique<airwin2rack::UltrasonicMed::UltrasonicMed>(0); }});
#include "autogen_airwin/UnBox.h"
int UnBox_unused = AirwinRegistry::registerAirwindow({"UnBox", "Saturation", 284, "UnBox is a distortion where only the harmonics that don't alias are allowed to distort.", airwin2rack::UnBox::kNumParameters, []() { return std::make_unique<airwin2rack::UnBox::UnBox>(0); }});
#include "autogen_airwin/VariMu.h"
int VariMu_unused = AirwinRegistry::registerAirwindow({"VariMu", "Dynamics", 162, "VariMu is a more organic variation on Pressure (a compressor)", airwin2rack::VariMu::kNumParameters, []() { return std::make_unique<airwin2rack::VariMu::VariMu>(0); }});
#include "autogen_airwin/Verbity.h"
int Verbity_unused = AirwinRegistry::registerAirwindow({"Verbity", "Unclassified", -1, "Verbity is a dual-mono reverb, which uses feedforward reverb topology.", airwin2rack::Verbity::kNumParameters, []() { return std::make_unique<airwin2rack::Verbity::Verbity>(0); }});
#include "autogen_airwin/Verbity2.h"
int Verbity2_unused = AirwinRegistry::registerAirwindow({"Verbity2", "Reverb", 260, "Verbity2 adds stereo crossmodulation and expands Verbity's feedforward reverb topology.", airwin2rack::Verbity2::kNumParameters, []() { return std::make_unique<airwin2rack::Verbity2::Verbity2>(0); }});
#include "autogen_airwin/Vibrato.h"
int Vibrato_unused = AirwinRegistry::registerAirwindow({"Vibrato", "Lo-Fi", 243, "Vibrato lets you vibrato, chorus, flange, and make odd FM noises.", airwin2rack::Vibrato::kNumParameters, []() { return std::make_unique<airwin2rack::Vibrato::Vibrato>(0); }});
#include "autogen_airwin/VinylDither.h"
int VinylDither_unused = AirwinRegistry::registerAirwindow({"VinylDither", "Dithers", 143, "VinylDither is a high-performance dither that converts digital noise to ‘groove noise’.", airwin2rack::VinylDither::kNumParameters, []() { return std::make_unique<airwin2rack::VinylDither::VinylDither>(0); }});
#include "autogen_airwin/VoiceOfTheStarship.h"
int VoiceOfTheStarship_unused = AirwinRegistry::registerAirwindow({"VoiceOfTheStarship", "Noise", 250, "VoiceOfTheStarship is a deep noise tone source.", airwin2rack::VoiceOfTheStarship::kNumParameters, []() { return std::make_unique<airwin2rack::VoiceOfTheStarship::VoiceOfTheStarship>(0); }});
#include "autogen_airwin/VoiceTrick.h"
int VoiceTrick_unused = AirwinRegistry::registerAirwindow({"VoiceTrick", "Utility", 357, "VoiceTrick lets you record vocals while monitoring over speakers.", airwin2rack::VoiceTrick::kNumParameters, []() { return std::make_unique<airwin2rack::VoiceTrick::VoiceTrick>(0); }});
#include "autogen_airwin/Weight.h"
int Weight_unused = AirwinRegistry::registerAirwindow({"Weight", "Filter", 211, "Weight is a very accurate sub-bass boost based on Holt.", airwin2rack::Weight::kNumParameters, []() { return std::make_unique<airwin2rack::Weight::Weight>(0); }});
#include "autogen_airwin/Wider.h"
int Wider_unused = AirwinRegistry::registerAirwindow({"Wider", "Stereo", 289, "Wider is Airwindows stereo space shaping.", airwin2rack::Wider::kNumParameters, []() { return std::make_unique<airwin2rack::Wider::Wider>(0); }});
#include "autogen_airwin/XBandpass.h"
int XBandpass_unused = AirwinRegistry::registerAirwindow({"XBandpass", "XYZ Filters", 375, "XBandpass is a distorted digital EQ, inspired by retro sampler DSP.", airwin2rack::XBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::XBandpass::XBandpass>(0); }});
#include "autogen_airwin/XHighpass.h"
int XHighpass_unused = AirwinRegistry::registerAirwindow({"XHighpass", "XYZ Filters", 376, "XHighpass is a distorted digital EQ, inspired by retro sampler DSP.", airwin2rack::XHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::XHighpass::XHighpass>(0); }});
#include "autogen_airwin/XLowpass.h"
int XLowpass_unused = AirwinRegistry::registerAirwindow({"XLowpass", "XYZ Filters", 377, "XLowpass is a distorted digital EQ, inspired by retro sampler DSP.", airwin2rack::XLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::XLowpass::XLowpass>(0); }});
#include "autogen_airwin/XNotch.h"
int XNotch_unused = AirwinRegistry::registerAirwindow({"XNotch", "XYZ Filters", 378, "XNotch is a distorted digital EQ, not as glitchy as the others.", airwin2rack::XNotch::kNumParameters, []() { return std::make_unique<airwin2rack::XNotch::XNotch>(0); }});
#include "autogen_airwin/XRegion.h"
int XRegion_unused = AirwinRegistry::registerAirwindow({"XRegion", "XYZ Filters", 379, "XRegion is distorted staggered bandpasses, for extreme soundmangling.", airwin2rack::XRegion::kNumParameters, []() { return std::make_unique<airwin2rack::XRegion::XRegion>(0); }});
#include "autogen_airwin/YBandpass.h"
int YBandpass_unused = AirwinRegistry::registerAirwindow({"YBandpass", "XYZ Filters", 368, "YBandpass is soft and smooth to nasty, edgy texture-varying filtering.", airwin2rack::YBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::YBandpass::YBandpass>(0); }});
#include "autogen_airwin/YHighpass.h"
int YHighpass_unused = AirwinRegistry::registerAirwindow({"YHighpass", "XYZ Filters", 370, "YHighpass is soft and smooth to nasty, edgy texture-varying filtering.", airwin2rack::YHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::YHighpass::YHighpass>(0); }});
#include "autogen_airwin/YLowpass.h"
int YLowpass_unused = AirwinRegistry::registerAirwindow({"YLowpass", "XYZ Filters", 372, "YLowpass is soft and smooth to nasty, edgy texture-varying filtering.", airwin2rack::YLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::YLowpass::YLowpass>(0); }});
#include "autogen_airwin/YNotBandpass.h"
int YNotBandpass_unused = AirwinRegistry::registerAirwindow({"YNotBandpass", "XYZ Filters", 369, "YNotBandpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwin2rack::YNotBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::YNotBandpass::YNotBandpass>(0); }});
#include "autogen_airwin/YNotHighpass.h"
int YNotHighpass_unused = AirwinRegistry::registerAirwindow({"YNotHighpass", "XYZ Filters", 371, "YNotHighpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwin2rack::YNotHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::YNotHighpass::YNotHighpass>(0); }});
#include "autogen_airwin/YNotLowpass.h"
int YNotLowpass_unused = AirwinRegistry::registerAirwindow({"YNotLowpass", "XYZ Filters", 373, "YNotLowpass is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwin2rack::YNotLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::YNotLowpass::YNotLowpass>(0); }});
#include "autogen_airwin/YNotNotch.h"
int YNotNotch_unused = AirwinRegistry::registerAirwindow({"YNotNotch", "Unclassified", -1, "YNotNotch is soft and smooth to nasty, edgy texture-varying filtering, no control smoothing.", airwin2rack::YNotNotch::kNumParameters, []() { return std::make_unique<airwin2rack::YNotNotch::YNotNotch>(0); }});
#include "autogen_airwin/YNotch.h"
int YNotch_unused = AirwinRegistry::registerAirwindow({"YNotch", "XYZ Filters", 374, "YNotch is soft and smooth to nasty, edgy texture-varying filtering.", airwin2rack::YNotch::kNumParameters, []() { return std::make_unique<airwin2rack::YNotch::YNotch>(0); }});
#include "autogen_airwin/ZBandpass.h"
int ZBandpass_unused = AirwinRegistry::registerAirwindow({"ZBandpass", "XYZ Filters", 363, "ZBandpass is a bandpass made to sound and act like the Emu e6400 Ultra bandpass.", airwin2rack::ZBandpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZBandpass::ZBandpass>(0); }});
#include "autogen_airwin/ZBandpass2.h"
int ZBandpass2_unused = AirwinRegistry::registerAirwindow({"ZBandpass2", "XYZ Filters", 358, "ZBandpass2 acts more like the Emu e6400 Ultra bandpass in motion, with control smoothing.", airwin2rack::ZBandpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZBandpass2::ZBandpass2>(0); }});
#include "autogen_airwin/ZHighpass.h"
int ZHighpass_unused = AirwinRegistry::registerAirwindow({"ZHighpass", "XYZ Filters", 364, "ZHighpass is a highpass made to sound and act like the Emu e6400 Ultra highpass.", airwin2rack::ZHighpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZHighpass::ZHighpass>(0); }});
#include "autogen_airwin/ZHighpass2.h"
int ZHighpass2_unused = AirwinRegistry::registerAirwindow({"ZHighpass2", "XYZ Filters", 359, "ZHighpass2 acts more like the Emu e6400 Ultra highpass in motion, with control smoothing.", airwin2rack::ZHighpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZHighpass2::ZHighpass2>(0); }});
#include "autogen_airwin/ZLowpass.h"
int ZLowpass_unused = AirwinRegistry::registerAirwindow({"ZLowpass", "XYZ Filters", 365, "ZLowpass is a lowpass made to sound and act like the Emu e6400 Ultra lowpass.", airwin2rack::ZLowpass::kNumParameters, []() { return std::make_unique<airwin2rack::ZLowpass::ZLowpass>(0); }});
#include "autogen_airwin/ZLowpass2.h"
int ZLowpass2_unused = AirwinRegistry::registerAirwindow({"ZLowpass2", "XYZ Filters", 360, "ZLowpass2 acts more like the Emu e6400 Ultra lowpass in motion, with control smoothing.", airwin2rack::ZLowpass2::kNumParameters, []() { return std::make_unique<airwin2rack::ZLowpass2::ZLowpass2>(0); }});
#include "autogen_airwin/ZNotch.h"
int ZNotch_unused = AirwinRegistry::registerAirwindow({"ZNotch", "XYZ Filters", 366, "ZNotch is a notch filter made to sound and act like the Emu e6400 Phaser.", airwin2rack::ZNotch::kNumParameters, []() { return std::make_unique<airwin2rack::ZNotch::ZNotch>(0); }});
#include "autogen_airwin/ZNotch2.h"
int ZNotch2_unused = AirwinRegistry::registerAirwindow({"ZNotch2", "XYZ Filters", 361, "ZNotch2 acts more like the Emu e6400 Ultra phaser in motion, with control smoothing.", airwin2rack::ZNotch2::kNumParameters, []() { return std::make_unique<airwin2rack::ZNotch2::ZNotch2>(0); }});
#include "autogen_airwin/ZOutputStage.h"
int ZOutputStage_unused = AirwinRegistry::registerAirwindow({"ZOutputStage", "Distortion", 125, "ZOutputStage is the output clipping from the Emu e6400 style Z filters.", airwin2rack::ZOutputStage::kNumParameters, []() { return std::make_unique<airwin2rack::ZOutputStage::ZOutputStage>(0); }});
#include "autogen_airwin/ZRegion.h"
int ZRegion_unused = AirwinRegistry::registerAirwindow({"ZRegion", "XYZ Filters", 367, "ZRegion is an Emu e6400 style Airwindows Region filter.", airwin2rack::ZRegion::kNumParameters, []() { return std::make_unique<airwin2rack::ZRegion::ZRegion>(0); }});
#include "autogen_airwin/ZRegion2.h"
int ZRegion2_unused = AirwinRegistry::registerAirwindow({"ZRegion2", "XYZ Filters", 362, "ZRegion2 is an Emu e6400 style Airwindows Region filter, with control smoothing.", airwin2rack::ZRegion2::kNumParameters, []() { return std::make_unique<airwin2rack::ZRegion2::ZRegion2>(0); }});
#include "autogen_airwin/curve.h"
int curve_unused = AirwinRegistry::registerAirwindow({"curve", "Dynamics", 164, "curve is the simplest, purest form of Recurve with no extra boosts.", airwin2rack::curve::kNumParameters, []() { return std::make_unique<airwin2rack::curve::curve>(0); }});
#include "autogen_airwin/kCathedral.h"
int kCathedral_unused = AirwinRegistry::registerAirwindow({"kCathedral", "Reverb", 268, "kCathedral is a giant reverby space.", airwin2rack::kCathedral::kNumParameters, []() { return std::make_unique<airwin2rack::kCathedral::kCathedral>(0); }});
#include "autogen_airwin/kCathedral2.h"
int kCathedral2_unused = AirwinRegistry::registerAirwindow({"kCathedral2", "Reverb", 255, "kCathedral2 is a giant reverby space modeled after the Bricasti Cathedral.", airwin2rack::kCathedral2::kNumParameters, []() { return std::make_unique<airwin2rack::kCathedral2::kCathedral2>(0); }});
#include "autogen_airwin/kChamberAR.h"
int kChamberAR_unused = AirwinRegistry::registerAirwindow({"kChamberAR", "Ambience", 11, "kChamberAR is a take on tape echo into chamber echo.", airwin2rack::kChamberAR::kNumParameters, []() { return std::make_unique<airwin2rack::kChamberAR::kChamberAR>(0); }});
#include "autogen_airwin/kPlateA.h"
int kPlateA_unused = AirwinRegistry::registerAirwindow({"kPlateA", "Reverb", 258, "kPlateA is a plate reverb, not unlike its namesake atop Abbey Road.", airwin2rack::kPlateA::kNumParameters, []() { return std::make_unique<airwin2rack::kPlateA::kPlateA>(0); }});
#include "autogen_airwin/kPlateB.h"
int kPlateB_unused = AirwinRegistry::registerAirwindow({"kPlateB", "Reverb", 257, "kPlateB is a plate reverb, not unlike its namesake atop Abbey Road.", airwin2rack::kPlateB::kNumParameters, []() { return std::make_unique<airwin2rack::kPlateB::kPlateB>(0); }});
#include "autogen_airwin/kPlateC.h"
int kPlateC_unused = AirwinRegistry::registerAirwindow({"kPlateC", "Reverb", 259, "kPlateC is a plate reverb, not unlike its namesake atop Abbey Road.", airwin2rack::kPlateC::kNumParameters, []() { return std::make_unique<airwin2rack::kPlateC::kPlateC>(0); }});
#include "autogen_airwin/kPlateD.h"
int kPlateD_unused = AirwinRegistry::registerAirwindow({"kPlateD", "Reverb", 256, "kPlateD is a plate reverb, not unlike its namesake atop Abbey Road.", airwin2rack::kPlateD::kNumParameters, []() { return std::make_unique<airwin2rack::kPlateD::kPlateD>(0); }});
#include "autogen_airwin/uLawDecode.h"
int uLawDecode_unused = AirwinRegistry::registerAirwindow({"uLawDecode", "Utility", 351, "uLawDecode is a Console-like encode/decode pair, but much more extreme.", airwin2rack::uLawDecode::kNumParameters, []() { return std::make_unique<airwin2rack::uLawDecode::uLawDecode>(0); }});
#include "autogen_airwin/uLawEncode.h"
int uLawEncode_unused = AirwinRegistry::registerAirwindow({"uLawEncode", "Utility", 350, "uLawEncode is a Console-like encode/decode pair, but much more extreme.", airwin2rack::uLawEncode::kNumParameters, []() { return std::make_unique<airwin2rack::uLawEncode::uLawEncode>(0); }});
int unusedComplete = AirwinRegistry::completeRegistry();
