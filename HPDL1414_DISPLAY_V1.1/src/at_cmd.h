/**
  ******************************************************************************
  * @file    at_cmd.h
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#ifndef _AT_CMD_H_
#define _AT_CMD_H_

/* MANDATORY to match the order of at_cmds_lens and at_cmds_lens. */
typedef enum
{
    AT_SET_LED,
    AT_GET_TIME,
    AT_SET_TIME,
    AT_SHOW_TIME,
    AT_SHOW_DATE,
    AT_SHOW_TEXT,
    AT_TOTAL_CMDS,
    AT_ERROR
}at_cmd_type_t;


bool AtCmdDataIn(uint8_t *data, uint32_t *data_len);

at_cmd_type_t AtCmdGetCmd(uint8_t *data, uint32_t *data_len, uint32_t len_offset);

void AtCmdDumpCmd(void);

#endif /* _AT_CMD_H_ */


