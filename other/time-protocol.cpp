#include "time-protocol.h"
TIME_PRO date_time(&uart2);
void uart_interrupt_event()
{
	date_time.get_char(date_time.usart->receive());
}


void TIME_PRO::begin(u32 baud_rate)
{
	usart->begin(baud_rate);
	usart->attach_interrupt(uart_interrupt_event);
	usart->interrupt(ENABLE);
}


void TIME_PRO::get_char(u8 ch)
{
	cmd_feed_dog();
	if(ch == '$')
	{
		cmd_num++;
	}		
	if(char_count>=BUF_SIZE)char_count = 0;//防止数据溢出
	buf[char_count++] = ch;
}


void	TIME_PRO::cmd_feed_dog()
{
	cmd_current_time = millis();
}
int	TIME_PRO::cmd_check_dog()
{
	if(millis() - cmd_current_time > CMD_TIMEOUT)	
	{
		return -1;
	}
	else
	{
		return 0;
	}

}
int TIME_PRO::find_str(u8 *str,u8 *ptr,u16 &seek)
{
	seek = 0;
	u8 *temp_str=NULL;
	u8 *temp_ptr=NULL;
	u8 *temp_char=NULL;
	if(0==str||0==ptr)
		return -2;
	for(temp_str=str;*temp_str!='\0';temp_str++)	 //依次查找字符串
	{
		temp_char=temp_str; //指向当前字符串
		//比较
		for(temp_ptr=ptr;*temp_ptr!='\0';temp_ptr++)
		{	
			if(*temp_ptr!=*temp_char)
			break;
			temp_char++;
		}
		if(*temp_ptr=='\0')  //出现了所要查找的字符，退出
		{
			return 0;
		}
		seek++;  //当前偏移量加1
	}
	return -1;
}

#include "stdio.h"
#include "string.h"

int TIME_PRO::process()
{
	u8 item_count = 0;
	u8 bd_info_item_seek = 0;
	u16 seek = 0;
	u8 ret = 0;
	u8 *p_cmd = NULL;
	int flag;
	//这个bug隐藏的太深了
	no_interrupts();
	flag = (cmd_check_dog() == -1) && (cmd_num > 0);
	interrupts();
	if( flag == 1 )

	{
		
		memset(&date_time,0x00,sizeof(date_time));//清除结构体	
		ret = find_str(buf,(u8 *)"$dmyhms",seek);
		if(ret == 0)
		{
			p_cmd = buf + seek;
			do
			{
				p_cmd++;
				switch(*p_cmd)
				{
					case ',':
						item_count++;
						bd_info_item_seek = 0;
						break;
					default :
						switch(item_count)
						{
							case 0:break;
							case 1:date_time[bd_info_item_seek]		= *p_cmd;break;
							default:break;								
						}		
						bd_info_item_seek++;						
						break;				
				}				
			}while(*p_cmd!='*');
		}
		
		memset(&cutdown_string,0x00,sizeof(cutdown_string));//清除结构体	
		ret = find_str(buf,(u8 *)"$cutdown",seek);
		item_count = 0;
		if(ret == 0)
		{
			p_cmd = buf + seek;
			do
			{
				p_cmd++;
				switch(*p_cmd)
				{
					case ',':
						item_count++;
						bd_info_item_seek = 0;
						break;
					default :
						switch(item_count)
						{
							case 0:break;
							case 1:cutdown_string[bd_info_item_seek]		= *p_cmd;break;
							default:break;								
						}		
						bd_info_item_seek++;						
						break;				
				}				
			}while(*p_cmd!='*');
		}
		memset(&playtime_string,0x00,sizeof(playtime_string));//清除结构体	
		ret = find_str(buf,(u8 *)"$playtime",seek);
		item_count = 0;
		if(ret == 0)
		{
			p_cmd = buf + seek;
			do
			{
				p_cmd++;
				switch(*p_cmd)
				{
					case ',':
						item_count++;
						bd_info_item_seek = 0;
						break;
					default :
						switch(item_count)
						{
							case 0:break;
							case 1:playtime_string[bd_info_item_seek]		= *p_cmd;break;
							default:break;								
						}		
						bd_info_item_seek++;						
						break;				
				}				
			}while(*p_cmd!='*');
		}
		date_time[12] = 0;
		char_count = 0;
		cmd_num = 0;
		
		uart1.printf((char *)buf);
		if(buf[1] != 'd')
			uart1.printf("flag = %d,cmd = %d",flag,cmd_num);
		uart1.printf("\r\n");
		memset(&buf,0x00,1024);//清除结构体	

		return 1;
	}
	return 0;
}

