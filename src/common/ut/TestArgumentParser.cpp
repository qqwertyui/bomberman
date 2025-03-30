#include <gtest/gtest.h>

#include "common/ArgumentParser.cpp"

namespace bomberman::common {
class TestArgumentParser : public ::testing::Test {
protected:
  ArgumentParser sut;
};

TEST_F(TestArgumentParser, testIfEmptyInput) {
  std::vector<std::string> args{};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 0);
}

TEST_F(TestArgumentParser, testIfNameOnlyArgument) {
  std::vector<std::string> args{"--test"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result["test"] == "");
}

TEST_F(TestArgumentParser, testIfInvalidArgumentPrefix) {
  std::vector<std::string> args{"-test", "asd"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 0);
}

TEST_F(TestArgumentParser, testIfNumberOfOutputElementsIsOk1) {
  std::vector<std::string> args{"--test", "asd"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result["test"] == "asd");
}

TEST_F(TestArgumentParser, testIfNameAndNameValueArgumentsAreTogether) {
  std::vector<std::string> args{"--dummy", "--test", "asd"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result["dummy"] == "");
  EXPECT_TRUE(result["test"] == "asd");
}

TEST_F(TestArgumentParser, testIfNumberOfOutputElementsIsOk2) {
  std::vector<std::string> args{"--test", "asd", "--path", "asd"};
  auto result = sut.parse(args);

  EXPECT_EQ(result.size(), 2);
}

TEST_F(TestArgumentParser, testIfCorrectlyMapped) {
  std::vector<std::string> args{"--test", "asd", "--path", "/home/admin/test"};
  auto result = sut.parse(args);

  EXPECT_TRUE(result["test"] == "asd");
  EXPECT_TRUE(result["path"] == "/home/admin/test");
}

TEST_F(TestArgumentParser, testIfNonExistentArgument) {
  std::vector<std::string> args{"--test", "asd"};
  auto result = sut.parse(args);

  EXPECT_TRUE(result["test"] == "asd");
  int i = 0;
  try {
    result.at("path");
    i = 1;
  } catch (const std::out_of_range &e) {
    i = 2;
  }
  EXPECT_EQ(i, 2);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // namespace bomberman::common