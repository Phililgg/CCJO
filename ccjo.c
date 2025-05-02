#include "ccjo.h"
#include <math.h>
#include <stdlib.h>

// 计算欧氏距离（可替换为其他距离度量）
static double distance(double* a, double* b, int dims) {
    double sum = 0.0;
    for(int i=0; i<dims; i++) {
        sum += pow(a[i]-b[i], 2);
    }
    return sqrt(sum);
}

// 通用潜力计算接口
static double complex calculate_potential(
    Agent* agent, 
    Target* target,
    CCJO_Config config,
    int iter,
    int total_iters
) {
    // 动态权重计算
    double alpha = config.weights.alpha_func(iter, total_iters);
    double beta = config.weights.beta_func(iter, total_iters);
    
    // 示例潜力模型（需根据实际问题实现）
    double distance_cost = distance(agent->position, target->attributes, 2);
    double base_value = target->attributes[0]; // 假设属性0为基础价值
    
    return alpha * (base_value - config.cost_factor * distance_cost) +
           beta * I * target->attributes[1]; // 属性1为次要价值
}

// 主优化逻辑
double ccjo_optimize(
    Agent* agents, int agent_count,
    Target* targets, int target_count,
    CCJO_Config config
) {
    double total_value = 0.0;
    
    for(int iter=0; iter<config.max_iter; iter++) {
        // 动态权重更新
        double alpha = config.weights.alpha_func(iter, config.max_iter);
        double beta = config.weights.beta_func(iter, config.max_iter);
        
        // 潜力计算与决策（示例简化逻辑）
        for(int a=0; a<agent_count; a++) {
            if(agents[a].current_load >= agents[a].capacity) continue;
            
            double max_potential = -INFINITY;
            int selected_target = -1;
            
            for(int t=0; t<target_count; t++) {
                if(targets[t].state & 0x1) continue; // 状态位0表示已分配
                
                double complex pot = calculate_potential(
                    &agents[a], &targets[t], config, iter, config.max_iter
                );
                
                // 取实部作为决策依据
                if(creal(pot) > max_potential) {
                    max_potential = creal(pot);
                    selected_target = t;
                }
            }
            
            if(selected_target != -1) {
                // 更新状态和总值
                targets[selected_target].state |= 0x1;
                agents[a].current_load++;
                total_value += creal(calculate_potential(
                    &agents[a], &targets[selected_target], config, iter, config.max_iter
                ));
            }
        }
    }
    
    return total_value;
}
