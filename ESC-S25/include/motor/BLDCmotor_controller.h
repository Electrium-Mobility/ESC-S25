
#define HIGH_IMPEDANCE 0


class motor_controller {
    public:
        motor_controller(int pin_A_PWM, int pin_B_PWM, int pin_C_PWM, float velocity_limit, float voltage_limit, float current_limit, FOC_MODULATION modulation)
        {
            this->pin_A_PWM = pin_A_PWM;
            this->pin_B_PWM = pin_B_PWM;
            this->pin_C_PWM = pin_C_PWM;
            this->velocity_limit = velocity_limit;
            this->voltage_limit = voltage_limit;
            this->current_limit = current_limit;
            this->modulation = modulation;
        }

    private:
        int pin_A_PWM; // PWM pin for phase A
        int pin_B_PWM; // PWM pin for phase B
        int pin_C_PWM; // PWM pin for phase C

        float velocity_limit; // angular velocity limit in rad/s

        float voltage_limit;

        float current_limit; // current limit in A

        FOC_MODULATION modulation; // FOC modulation type
};