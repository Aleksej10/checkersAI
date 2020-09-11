#include "model.hpp"

Model::Model() : 
    model_(Net()),
    optimizer_(torch::optim::Adam(model_->parameters()))
{
    std::cout << "Model initialized\n";
}

void Model::load(){
    torch::load(model_, "model.pt");
    model_->eval();
    torch::load(optimizer_, "optimizer.pt");
    std::cout << "Model loaded\n";
}

void Model::save(){
    torch::save(model_, "model.pt");
    torch::save(optimizer_, "optimizer.pt");
    std::cout << "Model saved\n";
}

void Model::load_clean(){
    torch::load(model_, "clean_model.pt");
    model_->eval();
    torch::load(optimizer_, "clean_optimizer.pt");
    std::cout << "Clean model loaded\n";
}

void Model::save_clean(){
    torch::save(model_, "clean_model.pt");
    torch::save(optimizer_, "clean_optimizer.pt");
    std::cout << "Clean model saved\n";
}


float Model::train(torch::Tensor ins, torch::Tensor outs){
    torch::Device device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);
    model_->to(device);
    ins.to(device);
    outs.to(device);

    model_->zero_grad();
    torch::Tensor ys = model_->forward(ins);
    torch::Tensor loss = torch::mse_loss(ys, outs);
    float ls = loss.item<float>();
    loss.backward();
    optimizer_.step();
    return ls;
}



Net Model::get_net(){
    return model_;
}
