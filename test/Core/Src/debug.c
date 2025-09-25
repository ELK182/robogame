#include "debug.h"
#include <stdarg.h>

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef *debug_huart;
static DebugLevel current_debug_level = DEBUG_LEVEL_INFO;

// ���ջ�����
static uint8_t rx_buffer[DEBUG_BUFFER_SIZE];
static uint16_t rx_index = 0;

/* Private function prototypes -----------------------------------------------*/
static void debug_send_string(char *str);

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize debug module
  * @param  huart: Pointer to a UART_HandleTypeDef structure
  * @retval None
  */
void Debug_Init(UART_HandleTypeDef *huart)
{
  debug_huart = huart;
  current_debug_level = DEBUG_LEVEL_INFO;
  
  // �������ڽ����ж�
  HAL_UART_Receive_IT(debug_huart, &rx_buffer[rx_index], 1);
  
  DEBUG_INFO("Debug module initialized\r\n");
  DEBUG_INFO("System ready - Track follower with ultrasonic\r\n");
}

/**
  * @brief  Process received command
  * @param  command: Command string to process
  * @retval None
  */
void Debug_ProcessCommand(char *command)
{
  // ȥ��ĩβ�Ļ��з�
  char *pos;
  if ((pos = strchr(command, '\r')) != NULL) *pos = '\0';
  if ((pos = strchr(command, '\n')) != NULL) *pos = '\0';
  
  DEBUG_INFO("Received command: %s\r\n", command);
  
  // ��������
  if (strcmp(command, "help") == 0) {
    debug_printf("Available commands:\r\n");
    debug_printf("  help      - Show this help\r\n");
    debug_printf("  status    - Show system status\r\n");
    debug_printf("  sensors   - Read sensor values\r\n");
    debug_printf("  level X   - Set debug level (0-4)\r\n");
    debug_printf("  stop      - Stop motors\r\n");
    debug_printf("  run       - Resume normal operation\r\n");
  }
  else if (strcmp(command, "status") == 0) {
    debug_printf("System status:\r\n");
    debug_printf("  Debug level: %d\r\n", current_debug_level);
    debug_printf("  Track follower: Active\r\n");
    debug_printf("  Ultrasonic: Initialized\r\n");
  }
  else if (strcmp(command, "sensors") == 0) {
    // ���ú������ʹ���������
    Debug_SendSensorData();
  }
  else if (strncmp(command, "level", 5) == 0) {
    int level = atoi(command + 6);
    if (level >= DEBUG_LEVEL_NONE && level <= DEBUG_LEVEL_VERBOSE) {
      debug_set_level(level);
      debug_printf("Debug level set to: %d\r\n", level);
    } else {
      debug_printf("Invalid debug level. Use 0-4.\r\n");
    }
  }
  else if (strcmp(command, "stop") == 0) {
    // ֹͣ���
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
    debug_printf("Motors stopped\r\n");
  }
  else if (strcmp(command, "run") == 0) {
    debug_printf("Resuming normal operation\r\n");
  }
  else {
    debug_printf("Unknown command: %s\r\n", command);
    debug_printf("Type 'help' for available commands\r\n");
  }
}

/**
  * @brief  printf-like function for debug output
  * @param  fmt: Format string
  * @retval None
  */
void debug_printf(const char *fmt, ...)
{
  if (debug_huart == NULL) return;
  
  char buffer[DEBUG_BUFFER_SIZE];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, DEBUG_BUFFER_SIZE, fmt, args);
  va_end(args);
  
  debug_send_string(buffer);
}

/**
  * @brief  Set debug level
  * @param  level: Debug level to set
  * @retval None
  */
void debug_set_level(DebugLevel level)
{
  current_debug_level = level;
}

/**
  * @brief  Get current debug level
  * @retval Current debug level
  */
DebugLevel debug_get_level(void)
{
  return current_debug_level;
}

/**
  * @brief  Send raw data over debug interface
  * @param  data: Pointer to data
  * @param  size: Size of data
  * @retval None
  */
void debug_send_data(uint8_t *data, uint16_t size)
{
  if (debug_huart != NULL) {
    HAL_UART_Transmit(debug_huart, data, size, 100);
  }
}

/**
  * @brief  Callback for received data
  * @retval None
  */
void debug_receive_callback(void)
{
  // ����Ƿ��յ���������Ի��з�������
  if (rx_buffer[rx_index] == '\r' || rx_buffer[rx_index] == '\n') {
    if (rx_index > 0) {
      rx_buffer[rx_index] = '\0'; // ��ֹ�ַ���
      Debug_ProcessCommand((char*)rx_buffer);
    }
    rx_index = 0;
  } else {
    rx_index++;
    if (rx_index >= DEBUG_BUFFER_SIZE) {
      rx_index = 0; // ��ֹ���������
    }
  }
  
  // �������������ж�
  HAL_UART_Receive_IT(debug_huart, &rx_buffer[rx_index], 1);
}

/**
  * @brief  Send sensor data for debugging
  * @retval None
  */
void Debug_SendSensorData(void)
{
  // ������Ҫ�������ʵ�ʴ�����ʵ������ȡ����
  // ����Ϊʾ������
  debug_printf("Sensor data:\r\n");
  debug_printf("  Ultrasonic distance: %d cm\r\n", 25); // ʾ��ֵ
  debug_printf("  Track sensors: [1, 1, 0, 1, 0]\r\n"); // ʾ��ֵ
}

/**
  * @brief  Send motor PWM values
  * @param  pwm1-4: PWM values for each motor
  * @retval None
  */
void Debug_SendMotorPWM(uint16_t pwm1, uint16_t pwm2, uint16_t pwm3, uint16_t pwm4)
{
  debug_printf("Motor PWM: CH1=%d, CH2=%d, CH3=%d, CH4=%d\r\n", 
               pwm1, pwm2, pwm3, pwm4);
}

/**
  * @brief  Send track status
  * @param  track_values: Array of track sensor values
  * @retval None
  */
void Debug_SendTrackStatus(uint8_t *track_values)
{
  debug_printf("Track status: ");
  for (int i = 0; i < 5; i++) {
    debug_printf("%d ", track_values[i]);
  }
  debug_printf("\r\n");
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Send string over debug interface
  * @param  str: String to send
  * @retval None
  */
static void debug_send_string(char *str)
{
  if (debug_huart != NULL) {
    HAL_UART_Transmit(debug_huart, (uint8_t*)str, strlen(str), 100);
  }
}