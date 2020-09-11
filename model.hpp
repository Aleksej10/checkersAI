#ifndef MODEL_H
#define MODEL_H

#include <torch/torch.h>

struct NetImpl : torch::nn::Module {
    NetImpl()
      : sq1(torch::nn::Conv2d(torch::nn::Conv2dOptions(4, 4, /*kernel_size=*/{3,2}).stride(1).padding(0)),
            torch::nn::ReLU(),
            torch::nn::MaxPool2d(torch::nn::MaxPool2dOptions(/*kernel_size*/2).stride(1).padding(0))),
        sq2(torch::nn::Conv2d(torch::nn::Conv2dOptions(4, 8, /*kernel_size=*/2).stride(1).padding(0)),
            torch::nn::ReLU(),
            torch::nn::MaxPool2d(torch::nn::MaxPool2dOptions(/*kernel_size*/{2,1}).stride(1).padding(0))),
        l1(24, 48),
        l2(48, 16),
        l3(16, 1) 
    {
        register_module("sq1", sq1);
        register_module("sq2", sq2);
        register_module("l1", l1);
        register_module("l2", l2);
        register_module("l3", l3);
    }

    torch::Tensor forward(torch::Tensor x) {
        x = sq1->forward(x);
        x = sq2->forward(x);
        x = x.reshape({x.size(0), -1});
        x = torch::nn::Dropout()->forward(x);
        x = torch::relu(l1->forward(x));
        x = torch::relu(l2->forward(x));
        x = torch::tanh(l3->forward(x));
        return x;
    }

    torch::nn::Sequential sq1;
    torch::nn::Sequential sq2;
    torch::nn::Linear l1;
    torch::nn::Linear l2;
    torch::nn::Linear l3;
};
TORCH_MODULE(Net);

class Model{
private:
    Net model_;
    torch::optim::Adam optimizer_;
public:
    Model();
    float train(torch::Tensor, torch::Tensor);
    void load();
    void save();
    void load_clean();
    void save_clean();
    Net get_net();
};


#endif
