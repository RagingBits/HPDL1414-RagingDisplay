/**
  ******************************************************************************
  * @file    at_cmd.c
  * @author  RagingBits
  * @version V1.0
  * @date    17-July-2021
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "at_cmd.h"

#define _AT_CMD_DATA_IN_BUFF_LEN     100U

#define _AT_CMD__START_BLOCK     "AT+"
#define _AT_CMD__START_BLOCK_LEN  3
#define _AT_CMD__END_BLOCK2       '\n'
#define _AT_CMD__END_BLOCK1       '\r'

#define _AT_CMD__LARGEST_CMD     10U

/* MANDATORY to match the order of _at_cmds and at_cmd_type_t. */
/* This is the main ruler of the order. So that the search algorithm works,
 * it needs to start from the smallest command string length to the largest. */
const uint16_t _at_cmds_lens[] =
{
    8U, /* Set led. */
    8U, /* Get time. */
    9U, /* Set time. */
    9U, /* Show time. */
    10U, /* Show date. */
    10U, /* Show text. */
};

/* MANDATORY to match the order of _at_cmds_lens and at_cmd_type_t. */
const uint8_t _at_cmds[] =
{
    "SET_LED:GET_TIMESET_TIME:SHOW_TIMESHOW_DATE:SHOW_TEXT:"
};

static uint8_t _at_cmd_state = 0;

static at_cmd_type_t _at_cmd_received = AT_ERROR;

static uint16_t _at_cmd_data_in_fill = 0;

static uint8_t _at_cmd_data_in[_AT_CMD_DATA_IN_BUFF_LEN];


bool _AtCmdCmp(uint8_t *a, uint8_t *b, uint16_t data_len)
{
    bool result = false;

    while(((*a) == (*b)) && (data_len > 0))
    {
        a++;
        b++;
        data_len--;
    };

    if(0U == data_len)
    {
        result = true;
    }

    return result;
};

bool _AtCmdFindHead(uint8_t *data, uint16_t *data_len)
{
    bool result = false;
    bool run = true;
    uint32_t counter = 0U;

    while((0U < (*data_len))&& run)
    {
        if(_at_cmd_data_in_fill >= _AT_CMD__START_BLOCK_LEN)
        {
            if(_AtCmdCmp(_AT_CMD__START_BLOCK, _at_cmd_data_in, _AT_CMD__START_BLOCK_LEN))
            {
                _at_cmd_data_in_fill = 0U;

                run = false;
                result = true;
            }
            else
            {
                counter = 0;
                while(counter < (_AT_CMD__START_BLOCK_LEN-1))
                {
                    _at_cmd_data_in[counter] = _at_cmd_data_in[counter+1];
                    counter++;
                }

                _at_cmd_data_in[counter] = *data++;
                (*data_len)--;
            }
        }
        else
        {
            _at_cmd_data_in[_at_cmd_data_in_fill++] = *data++;
            (*data_len)--;
        }
    }

    return result;
}

at_cmd_type_t _AtCmdFindCmd(uint8_t *data, uint16_t *data_len)
{
    at_cmd_type_t result = AT_TOTAL_CMDS;
    uint32_t counter = 0;
    uint8_t *cmds_p = &_at_cmds[0U];

    while(counter < AT_TOTAL_CMDS)
    {
        if(_at_cmd_data_in_fill >= _at_cmds_lens[counter])
        {
            if(_AtCmdCmp(cmds_p, _at_cmd_data_in, _at_cmds_lens[counter]))
            {
                _at_cmd_data_in_fill = 0;
                result = (at_cmd_type_t)counter;
                counter = AT_TOTAL_CMDS;
            }
            else
            {
                cmds_p += _at_cmds_lens[counter];
                counter++;
            }
        }
        else
        {
            if(0 != (*data_len))
            {
                _at_cmd_data_in[_at_cmd_data_in_fill++] = *data++;
                (*data_len)--;
            }
            else
            {
                counter = AT_TOTAL_CMDS;
            }
        }
    }

    if((result == AT_TOTAL_CMDS) && (_at_cmd_data_in_fill >= _AT_CMD__LARGEST_CMD))
    {
        /* An error was found. */
        result = AT_ERROR;
        _at_cmd_data_in_fill = 0U;
        _at_cmd_state = 0;
    }

    return result;
}

bool _AtCmdExtractCmd(uint8_t *data, uint16_t *data_len)
{
    bool found = false;

    while((0U < (*data_len)) && (_at_cmd_data_in_fill < _AT_CMD_DATA_IN_BUFF_LEN) && !found)
    {
        if((*data == _AT_CMD__END_BLOCK2) ||
           (*data == _AT_CMD__END_BLOCK1))
        {
            found = true;

        }
        else
        {
            _at_cmd_data_in[_at_cmd_data_in_fill++] = *data++;
            (*data_len)--;
        }

    }

    return found;
}

bool AtCmdDataIn(uint8_t *data, uint32_t *data_len)
{
    bool result = false;
    uint32_t data_length = *data_len;

    switch(_at_cmd_state)
    {
        /* Find header. */
        case 0:
        {
            uint32_t temp = (data_length - *data_len);
            if(_AtCmdFindHead(&data[temp], data_len))
            {
                _at_cmd_state++;
            }
            else
            {
                break;
            }
        }
        /* No break on purpose. */

        /* Find command. */
        case 1:
        {
            uint32_t temp = (data_length - *data_len);
            _at_cmd_received = _AtCmdFindCmd(&data[temp], data_len);
            if(AT_TOTAL_CMDS > _at_cmd_received)
            {
                _at_cmd_state++;
            }
            else
            {
                break;
            }
        }
        /* No break on purpose. */

        /* Get command data. */
        case 2:
        {
            uint32_t temp = (data_length - *data_len);
            if(_AtCmdExtractCmd(&data[temp], data_len))
            {
                _at_cmd_state++;
                result = true;
            }
            else
            {
                break;
            }
        }
        /* No break on purpose. */

        case 3:
        default:
        {
            /* Ignore data until proper handling. */
        }
        break;
    }

    return result;
}



at_cmd_type_t AtCmdGetCmd(uint8_t *data, uint32_t *data_len, uint32_t len_offset)
{
    at_cmd_type_t result = AT_ERROR;
    uint32_t counter = 0;
    if(3U == _at_cmd_state)
    {
        if(len_offset <= _at_cmd_data_in_fill)
        {
            uint8_t *data_p =  &_at_cmd_data_in[len_offset];

            while(((*data_len)-- > 0) && (counter < _at_cmd_data_in_fill))
            {
                *data = *data_p;
                data++;
                data_p++;
                counter++;
            }

            result = _at_cmd_received;
            *data_len = counter;
        }
    }

    return result;
}


void AtCmdDumpCmd(void)
{
    _at_cmd_received = AT_ERROR;
    _at_cmd_state = 0U;
    _at_cmd_data_in_fill = 0;
}


