#include "main.h"

#define LEVER_MOTOR_PORT 10					// Lever motor
#define LEFT_BACK_MOTOR_PORT 6			// Drivetrain motors
#define RIGHT_BACK_MOTOR_PORT 5
#define LEFT_FRONT_MOTOR_PORT 16
#define RIGHT_FRONT_MOTOR_PORT 15
#define LEFT_INTAKE_MOTOR_PORT 18		// Intake motors
#define RIGHT_INTAKE_MOTOR_PORT 13


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Initialize LCD
	// TODO: Fix this so that it actually works
	pros::lcd::initialize();
	pros::lcd::set_text(2, "1011X_Alpha v0.2 - Kinda PushBot");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// Initialize motors
	// Note: all motors on right side + lever motor + intake motor are reversed.
	pros::Motor lever (LEVER_MOTOR_PORT, MOTOR_GEARSET_36, true);
	pros::Motor left_back_wheel (LEFT_BACK_MOTOR_PORT);
	pros::Motor right_back_wheel (RIGHT_BACK_MOTOR_PORT, true);
	pros::Motor left_front_wheel (LEFT_FRONT_MOTOR_PORT);
	pros::Motor right_front_wheel (RIGHT_FRONT_MOTOR_PORT, true);
	pros::Motor left_intake (LEFT_INTAKE_MOTOR_PORT, true);
	pros::Motor right_intake (RIGHT_INTAKE_MOTOR_PORT, true);

	// Set brake modes
	lever.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	left_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	left_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);

	// 1 point autonomous
	left_front_wheel.move_velocity(100);	// Move forwards
	left_back_wheel.move_velocity(100);
	right_front_wheel.move_velocity(101);
	right_back_wheel.move_velocity(101);
	pros::delay(3000); 										// Move for ~3 seconds
	left_front_wheel.move_velocity(0);		// Stop
	left_back_wheel.move_velocity(0);
	right_front_wheel.move_velocity(0);
	right_back_wheel.move_velocity(0);

	// Move back
	left_front_wheel.move_velocity(-100);
	left_back_wheel.move_velocity(-100);
	right_front_wheel.move_velocity(-100);
	right_back_wheel.move_velocity(-100);
	pros::delay(2000);										// Move for ~2 seconds
	left_front_wheel.move_velocity(0);		// Stop
	left_back_wheel.move_velocity(0);
	right_front_wheel.move_velocity(0);
	right_back_wheel.move_velocity(0);

	// Activate tray
	lever.move_velocity(100);
	pros::delay(1000);
	lever.move_velocity(0);
	lever.move_velocity(-50);
	pros::delay(1000);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	// Initialize motors
	// Note: all motors on right side + lever motor + intake motor are reversed.
	pros::Motor lever (LEVER_MOTOR_PORT, MOTOR_GEARSET_36, true);
	pros::Motor left_back_wheel (LEFT_BACK_MOTOR_PORT);
	pros::Motor right_back_wheel (RIGHT_BACK_MOTOR_PORT, true);
	pros::Motor left_front_wheel (LEFT_FRONT_MOTOR_PORT);
	pros::Motor right_front_wheel (RIGHT_FRONT_MOTOR_PORT, true);
	pros::Motor left_intake (LEFT_INTAKE_MOTOR_PORT, true);
	pros::Motor right_intake (RIGHT_INTAKE_MOTOR_PORT, true);

	// Set brake modes
	lever.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	left_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_back_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	left_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);
	right_front_wheel.set_brake_mode (pros::E_MOTOR_BRAKE_BRAKE);

	// Initialize controller
	pros::Controller master(pros::E_CONTROLLER_MASTER);

	// Drivetrain mode
	int DRIVETRAIN_MODE = 0;		// 0 = fast, default; 1 = slow

	// Main loop
	while (true) {
		// Get joystick input
		int LEFT_Y = master.get_analog(ANALOG_LEFT_Y);
		int RIGHT_Y = master.get_analog(ANALOG_RIGHT_Y);

		// Switch drivetrain mode
		if (master.get_digital(DIGITAL_Y)) {
			if (DRIVETRAIN_MODE == 0) {
				DRIVETRAIN_MODE = 1;
			}
			else if (DRIVETRAIN_MODE == 1) {
				DRIVETRAIN_MODE = 0;
			}
			pros::delay(50);
		}

		// Run motors based on joystick input (tank drive)
		// Slow/fast drivetrain mode is also implemented here
		if (DRIVETRAIN_MODE == 0) {
			left_front_wheel.move(LEFT_Y);
			left_back_wheel.move(LEFT_Y);

			right_front_wheel.move(RIGHT_Y);
			right_back_wheel.move(RIGHT_Y);
		}
		else if (DRIVETRAIN_MODE == 1) {
			left_front_wheel.move(LEFT_Y / 5);
			left_back_wheel.move(LEFT_Y / 5);

			right_front_wheel.move(RIGHT_Y / 5);
			right_back_wheel.move(RIGHT_Y / 5);
		}

		// Read buttons and run lever
		if (master.get_digital(DIGITAL_L1)) {
			lever.move_velocity(40);
		}
		else if (master.get_digital(DIGITAL_L2)) {
			lever.move_velocity(-40);
		}
		else {
			lever.move_velocity(0);
		}

		// Read buttons and run intake
		if (master.get_digital(DIGITAL_R1)) {
			left_intake.move_velocity(200);
			right_intake.move_velocity(200);
		}
		else if (master.get_digital(DIGITAL_R2)) {
			left_intake.move_velocity(200);
			right_intake.move_velocity(-200);
		}
		else {
			left_intake.move_velocity(0);
			right_intake.move_velocity(0);
		}

		// For autonomous testing
		if (master.get_digital(DIGITAL_A)) {
			autonomous();
		}

		pros::delay(2);		// Prevent infinite loop
	}
}
