
#ifndef _PLUG_DETECT_H_
#define _PLUG_DETECT_H_

/**
 * 插入检测初始化
 */
void plug_det_init(void);

/**
 * 获取插入状态
 * @return
 *     0 -- 未插入
 *     1 -- 插入但未由12V触发
 *     2 -- 插入且已由12V触发
 */
int plug_det_state(void);

#endif

