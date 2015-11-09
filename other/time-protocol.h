#ifndef __TIME_PROTOCOL
#define __TIME_PROTOCOL
#include "ebox.h"
#define CMD_TIMEOUT 5//超过5ms没有获得命令数据，认为命令接收完成.这个值应该根据串口波特率设置
#define BUF_SIZE 1024//命令缓冲区大小，根据自己的需求定义。

class TIME_PRO
{
	public:
		u8 		date_time[13];//"ddmmyyhhmmss"
		u8 		cutdown_string[10];//"dddhhmmss"
		u8 		playtime_string[7];//"hhmmss"
		u8		buf[BUF_SIZE];
	public:
		TIME_PRO(USART *p_usart)//传递秒中断的io接口
		{
			usart = p_usart;
		}
		void	begin(u32 baud_rate);
		int	process();//循环处理接收到的帧内容，如果有接收完成的命令，开始解析命令

	private:
		u32		cmd_current_time;
		u8		cmd_num;
		u16		char_count;

		USART *usart;
	
		int		find_str(u8 *str,u8 *ptr,u16 &seek);

		void	cmd_feed_dog();//命令接受喂狗，保持命令在接受完成之后才被process处理.
		int		cmd_check_dog();//检查命令超时，如果超时说明命令结束。
		void	get_char(u8 ch);//串口中断填充BD数据
		friend void uart_interrupt_event();//友元函数，需要访问私有成员get_char();

};
extern TIME_PRO date_time;

#endif
