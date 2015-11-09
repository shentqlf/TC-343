#ifndef __TIME_PROTOCOL
#define __TIME_PROTOCOL
#include "ebox.h"
#define CMD_TIMEOUT 5//����5msû�л���������ݣ���Ϊ����������.���ֵӦ�ø��ݴ��ڲ���������
#define BUF_SIZE 1024//���������С�������Լ��������塣

class TIME_PRO
{
	public:
		u8 		date_time[13];//"ddmmyyhhmmss"
		u8 		cutdown_string[10];//"dddhhmmss"
		u8 		playtime_string[7];//"hhmmss"
		u8		buf[BUF_SIZE];
	public:
		TIME_PRO(USART *p_usart)//�������жϵ�io�ӿ�
		{
			usart = p_usart;
		}
		void	begin(u32 baud_rate);
		int	process();//ѭ��������յ���֡���ݣ�����н�����ɵ������ʼ��������

	private:
		u32		cmd_current_time;
		u8		cmd_num;
		u16		char_count;

		USART *usart;
	
		int		find_str(u8 *str,u8 *ptr,u16 &seek);

		void	cmd_feed_dog();//�������ι�������������ڽ������֮��ű�process����.
		int		cmd_check_dog();//������ʱ�������ʱ˵�����������
		void	get_char(u8 ch);//�����ж����BD����
		friend void uart_interrupt_event();//��Ԫ��������Ҫ����˽�г�Աget_char();

};
extern TIME_PRO date_time;

#endif
