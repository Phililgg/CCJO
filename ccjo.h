#ifndef CCJO_H
#define CCJO_H

#include <stdint.h>
#include <complex.h>

// 通用代理结构（可代表无人机、车辆等）
typedef struct {
    int id;
    double* position;     // 位置坐标数组（维度可扩展）
    int capacity;         // 最大任务容量
    int current_load;     // 当前任务数
} Agent;

// 通用任务目标结构
typedef struct {
    int id;
    double* attributes;   // 属性数组（如价值、位置等）
    uint32_t state;       // 状态位掩码（32位可扩展）
} Target;

// 动态权重参数
typedef struct {
    double (*alpha_func)(int, int);  // 权重计算函数指针
    double (*beta_func)(int, int);
} WeightPolicy;

// CCJO核心配置
typedef struct {
    int max_iter;         // 最大迭代次数
    double cost_factor;   // 距离成本系数
    WeightPolicy weights; // 权重策略
} CCJO_Config;

// 核心优化函数
double ccjo_optimize(
    Agent* agents, int agent_count,
    Target* targets, int target_count,
    CCJO_Config config
);

#endif
