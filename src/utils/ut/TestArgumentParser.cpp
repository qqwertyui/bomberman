#include <gtest/gtest.h>

#include "utils/ArgumentParser.cpp"

namespace SimpleSnake {
class TestArgumentParser : public ::testing::Test {
protected:
  ArgumentParser sut;
};

TEST_F(TestArgumentParser, testIfNumberOfOutputElementsIsOk1) {
  std::vector<std::string> args{"--test=asd", "-c"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 2);
}

TEST_F(TestArgumentParser, testIfNumberOfOutputElementsIsOk2) {
  std::vector<std::string> args{"--test=asd", "c", "-d", "123"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 4);
}

TEST_F(TestArgumentParser, testIfCorrectlyMapped) {
  std::vector<std::string> args{"--test=asd", "c",       "-d",
                                "123",        "-c-=aaa", "-d---=--fsd"};
  auto result = sut.parse(args);

  EXPECT_TRUE(result["test"] == "asd");
  EXPECT_TRUE(result["c"] == "");
  EXPECT_TRUE(result["d"] == "");
  EXPECT_TRUE(result["123"] == "");
  EXPECT_TRUE(result["c-"] == "aaa");
  EXPECT_TRUE(result["d---"] == "--fsd");
}

TEST_F(TestArgumentParser, testIfNonExistentArgument) {
  std::vector<std::string> args{"--test=asd"};
  auto result = sut.parse(args);

  EXPECT_TRUE(result["test"] == "asd");
  try {
    result.at("--test");
    EXPECT_TRUE(false);
  } catch (const std::out_of_range &e) {
  }
  try {
    result.at("-test");
    EXPECT_TRUE(false);
  } catch (const std::out_of_range &e) {
  }
}

TEST_F(TestArgumentParser, testIfContainsEqualsSign) {
  std::vector<std::string> args{"--test==asd"};
  auto result = sut.parse(args);

  EXPECT_TRUE(result["test"] == "=asd");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // namespace SimpleSnake