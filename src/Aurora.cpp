#include "plugin.hpp"
#include "engine.h"

struct Aurora : Module {

   enum ParamIds {
      KNOB1,
      KNOB2,
      KNOB3,
      KNOB4,
      KNOB5,
      KNOB6,
      ENCODER1,
      ENCODER2,
      NUM_PARAMS
   };
   enum InputIds {
      MOD_IN1,
      MOD_IN2,
      MOD_IN3,
      MOD_IN4,
      MOD_IN5,
      MOD_IN6,
      MOD_IN7,
      MOD_IN8,

      GATE_IN1,
      GATE_IN2,
      NUM_INPUTS
   };
   enum OutputIds {
      OUT1,
      OUT2,
      OUT3,
      OUT4,

      AUDIO_L,
      AUDIO_R,

      NUM_OUTPUTS
   };
   enum LightIds {
      LED_GATE1,
      LED_GATE2,
      LED_CV0,
      LED_CV1,
      LED_CV2,
      LED_CV3,
      NUM_LIGHTS
    };

   Aurora_process_type aurora;

   Aurora();
   void process(const ProcessArgs &args) override;
};

Aurora::Aurora() {
   config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

   configParam(Aurora::KNOB1, 0.0, 1.0, 0.5, "Knob 1", " %", 0.0f, 100.f);
   configParam(Aurora::KNOB2, 0.0, 1.0, 0.5, "Knob 2", " %", 0.0f, 100.f);
   configParam(Aurora::KNOB3, 0.0, 1.0, 0.5, "Knob 3", " %", 0.0f, 100.f);
   configParam(Aurora::KNOB4, 0.0, 1.0, 0.5, "Knob 4", " %", 0.0f, 100.f);
   configParam(Aurora::KNOB5, 0.0, 1.0, 0.5, "Knob 5", " %", 0.0f, 100.f);
   configParam(Aurora::KNOB6, 0.0, 1.0, 0.5, "Knob 6", " %", 0.0f, 100.f);
   configParam(Aurora::ENCODER1, 0.0, 1.0, 0.5, "Encoder 1", " %", 0.0f, 100.f);
   configParam(Aurora::ENCODER2, 0.0, 1.0, 0.5, "Encoder 2", " %", 0.0f, 100.f);

   Aurora_process_init(aurora);
}

void Aurora::process(const ProcessArgs &args) {
   // Reads all the input values and normalizes the values
   
   float in1 = inputs[MOD_IN1].getVoltage() / 10.0f;
   float in2 = inputs[MOD_IN2].getVoltage() / 10.0f;
   float in3 = inputs[MOD_IN3].getVoltage() / 10.0f;
   float in4 = inputs[MOD_IN4].getVoltage() / 10.0f;
   float in5 = inputs[MOD_IN5].getVoltage() / 10.0f;
   float in6 = inputs[MOD_IN6].getVoltage() / 10.0f;
   float in7 = inputs[MOD_IN7].getVoltage() / 10.0f;
   float in8 = inputs[MOD_IN8].getVoltage() / 10.0f;

   float gate1 = inputs[GATE_IN1].getVoltage() / 10.0f;
   float gate2 = inputs[GATE_IN2].getVoltage() / 10.0f;
   
   float mod_in1 = inputs[MOD_IN1].getVoltage() / 10.0f;
   float mod_in2 = inputs[MOD_IN2].getVoltage() / 10.0f;
   float mod_in3 = inputs[MOD_IN3].getVoltage() / 10.0f;
   float mod_in4 = inputs[MOD_IN4].getVoltage() / 10.0f;

   {
      float knob1 = params[KNOB1].getValue();
      float knob2 = params[KNOB2].getValue();
      float knob3 = params[KNOB3].getValue();
      float knob4 = params[KNOB4].getValue();
      float knob5 = params[KNOB5].getValue();
      float knob6 = params[KNOB6].getValue();
      float encoder1 = params[ENCODER1].getValue();
      float encoder2 = params[ENCODER2].getValue();

      Aurora_setParam1(aurora, knob1);
      Aurora_setParam2(aurora, knob2);
      Aurora_setParam3(aurora, knob3);
      Aurora_setParam4(aurora, knob4);
      Aurora_setParam5(aurora, knob5);
      Aurora_setParam6(aurora, knob6);
      
   }

   Aurora_process(aurora, in1, in2, in3, in4, in5, in6, in7, in8, args.sampleRate);

   outputs[AUDIO_L].setVoltage(Aurora_process_ret_0(aurora) * 10.0f);
   outputs[AUDIO_R].setVoltage(Aurora_process_ret_1(aurora) * 10.0f);
   
   outputs[OUT1].setVoltage(Aurora_process_ret_2(aurora) * 10.0f);
   outputs[OUT2].setVoltage(Aurora_process_ret_3(aurora) * 10.0f);
   outputs[OUT3].setVoltage(Aurora_process_ret_4(aurora) * 10.0f);
   outputs[OUT4].setVoltage(Aurora_process_ret_5(aurora) * 10.0f);


   lights[LED_CV0].setBrightness(Aurora_process_ret_2(aurora));
   lights[LED_CV1].setBrightness(Aurora_process_ret_3(aurora));
   lights[LED_CV2].setBrightness(Aurora_process_ret_4(aurora));
   lights[LED_CV3].setBrightness(Aurora_process_ret_5(aurora));

   lights[LED_GATE1].setBrightness(gate1);
   lights[LED_GATE2].setBrightness(gate2);
}

struct AuroraWidget : ModuleWidget {
   AuroraWidget(Aurora *module) {
      setModule(module);

      setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Aurora.svg")));

      addChild(createWidget<ScrewBlack>(Vec(15, 3)));
      addChild(createWidget<ScrewBlack>(Vec(box.size.x - 30, 3)));
      addChild(createWidget<ScrewBlack>(Vec(15, 367)));
      addChild(createWidget<ScrewBlack>(Vec(box.size.x - 30, 367)));

      addParam(createParam<RoundSmallBlackKnob>(Vec(14, 67), module, Aurora::ENCODER1));
      addParam(createParam<RoundSmallBlackKnob>(Vec(171, 67), module, Aurora::ENCODER1));

      addParam(createParam<Rogan1PWhite>(Vec(15, 135), module, Aurora::KNOB1)); // A
      addParam(createParam<Rogan1PWhite>(Vec(15, 180), module, Aurora::KNOB2)); // B
      addParam(createParam<Rogan1PWhite>(Vec(88, 108), module, Aurora::KNOB3)); // C
      addParam(createParam<Rogan1PWhite>(Vec(88, 185), module, Aurora::KNOB4)); // D
      addParam(createParam<Rogan1PWhite>(Vec(161, 135), module, Aurora::KNOB5)); // E
      addParam(createParam<Rogan1PWhite>(Vec(161, 180), module, Aurora::KNOB6)); // F


      for (int i = 0; i < 4; i++) {
         addInput(createInput<PJ301MPort>(Vec(8 + (26 * i), 242), module, Aurora::MOD_IN1 + i));
         addInput(createInput<PJ301MPort>(Vec(8 + (26 * i), 272), module, Aurora::MOD_IN5 + i));
      }
      addInput(createInput<PJ301MPort>(Vec(14, 106), module, Aurora::GATE_IN1));
      addInput(createInput<PJ301MPort>(Vec(170, 106), module, Aurora::GATE_IN2));

      addChild(createLightCentered<MediumLight<GreenLight>>(Vec(52, 117), module, Aurora::LED_GATE1));
      addChild(createLightCentered<MediumLight<GreenLight>>(Vec(156, 117), module, Aurora::LED_GATE2));

      //addLight(createLight<SmallLight<GreenLight>>(Vec(14, 100), module, Aurora::LED_GATE1));
      addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(43.70, 78.04)), module, Aurora::LED_CV0));
      addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(43.70, 78.04 + 26.3)), module, Aurora::LED_CV1));
      addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(43.70 + 8.89, 78.04)), module, Aurora::LED_CV2));
      addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(43.70 + 8.89, 78.04 + 26.3)), module, Aurora::LED_CV3));

      addOutput(createOutput<PJ301MPort>(Vec(117, 242), module, Aurora::OUT1));
      addOutput(createOutput<PJ301MPort>(Vec(117, 272), module, Aurora::OUT2));
      addOutput(createOutput<PJ301MPort>(Vec(117+26, 242), module, Aurora::OUT3));
      addOutput(createOutput<PJ301MPort>(Vec(117+26, 272), module, Aurora::OUT4));

      addOutput(createOutput<PJ301MPort>(Vec(174, 242), module, Aurora::AUDIO_L));
      addOutput(createOutput<PJ301MPort>(Vec(174, 272), module, Aurora::AUDIO_R));
   }
   
};

Model *aurora = createModel<Aurora, AuroraWidget>("Aurora");
