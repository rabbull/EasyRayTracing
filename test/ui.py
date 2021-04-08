import unittest
import sys


class UITestCase(unittest.TestCase):
    def test_argument_parsing(self):
        sys.argv = ["erf", "-h"]
        self.assertEqual()


if __name__ == '__main__':
    unittest.main()
